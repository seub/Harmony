#include "h2mesh.h"

#include "h2isometry.h"
#include "h2trianglesubdivision.h"
#include "h2meshconstructor.h"


H2Mesh::H2Mesh(const IsomH2Representation &rho, int depth) : rho(rho), depth(depth)
{
    //clock_t start = clock();
    fundamentalDomain = rho.generatePolygon(100);
    //clock_t end = clock();
    //std::cout << "Time to generate fundamental domain: " << (end - start)*1.0/CLOCKS_PER_SEC << "s" << std::endl;

    H2MeshConstructor(this);

    std::cout << "In mesh constructor: mesh pointer is " << this << std::endl;
}

H2Mesh::H2Mesh(const H2Mesh &other)
{
    rho = other.rho;
    depth = other.depth;

    fundamentalDomain = other.fundamentalDomain;
    fundamentalSteinerDomain = other.fundamentalSteinerDomain;

    subdivisions = other.subdivisions;
    triangles = other.triangles;

    regularPoints = other.regularPoints;
    cutPoints = other.cutPoints;
    boundaryPoints = other.boundaryPoints;
    vertexPoints = other.vertexPoints;
    steinerPoints = other.steinerPoints;

    createMeshPointsVector();
}

void H2Mesh::createMeshPointsVector()
{
    meshPoints.clear();

    for (auto &point : regularPoints)
    {
        meshPoints.push_back(&point);
    }

    for (auto &point : cutPoints)
    {
        meshPoints.push_back(&point);
    }

    for (auto &point : boundaryPoints)
    {
        meshPoints.push_back(&point);
    }

    for (auto &point : vertexPoints)
    {
        meshPoints.push_back(&point);
    }

    for (auto &point : steinerPoints)
    {
        meshPoints.push_back(&point);
    }
}

void swap(H2Mesh &first, H2Mesh &second)
{
    std::swap(first.rho, second.rho);
    std::swap(first.depth, second.depth);

    std::swap(first.fundamentalDomain, second.fundamentalDomain);
    std::swap(first.fundamentalSteinerDomain, second.fundamentalSteinerDomain);

    std::swap(first.subdivisions, second.subdivisions);
    std::swap(first.triangles, second.triangles);

    std::swap(first.meshPoints, second.meshPoints);
    std::swap(first.regularPoints, second.regularPoints);
    std::swap(first.cutPoints, second.cutPoints);
    std::swap(first.boundaryPoints, second.boundaryPoints);
    std::swap(first.vertexPoints, second.vertexPoints);
    std::swap(first.steinerPoints, second.steinerPoints);
}

H2Mesh & H2Mesh::operator=(H2Mesh other)
{
    swap(*this, other);
    return *this;
}


const std::vector<H2TriangleSubdivision> & H2Mesh::getSubdivisions() const
{
    return subdivisions;
}

bool H2Mesh::triangleContaining(const H2Point &point, H2Triangle &outputTriangle) const
{
    for(const auto &S : subdivisions)
    {
        if (S.triangleContaining(point, outputTriangle))
        {
            return true;
        }
    }
    return false;
}

bool H2Mesh::triangleContaining(const H2Point &point, H2Triangle &outputTriangle,
                                             int &meshIndex1, int &meshIndex2, int &meshIndex3) const
{
    for(const auto &S : subdivisions)
    {
        if (S.triangleContaining(point, outputTriangle, meshIndex1, meshIndex2, meshIndex3))
        {
            return true;
        }
    }
    return false;
}

const H2Point & H2Mesh::getH2Point(int index) const
{
    return subdivisions[meshPoints[index]->subdivisionIndex].points->at(meshPoints[index]->indexInSubdivision);
}

std::vector<H2Point> H2Mesh::getH2Neighbors(int index) const
{
    std::vector<int> neighborIndices = meshPoints.at(index)->neighborsIndices;
    std::vector<H2Point> res;
    for (auto k : neighborIndices)
    {
        res.push_back(getH2Point(k));
    }
    return res;
}

std::vector<H2Point> H2Mesh::getKickedH2Neighbors(int index) const
{
    H2MeshPoint* point = meshPoints.at(index);
    std::vector<int> neighborIndices = point->neighborsIndices;
    std::vector<H2Point> res = getH2Neighbors(index);

    if (point->isBoundaryPoint())
    {
        std::vector<H2Isometry> isometries = rho.evaluateRepresentation(((H2MeshBoundaryPoint*) point)->neighborsPairings);
        for (unsigned int i=0; i<neighborIndices.size(); ++i)
        {
            res[i] = isometries[i]*res[i];
        }
    }
    else if (point->isVertexPoint())
    {
        std::vector<H2Isometry> isometries = rho.evaluateRepresentation(((H2MeshVertexPoint*) point)->neighborsPairings);
        for (unsigned int i=0; i<neighborIndices.size(); ++i)
        {
            res[i] = isometries[i]*res[i];
        }
    }
    else if (point->isSteinerPoint())
    {
        std::vector<H2Isometry> isometries = rho.evaluateRepresentation(((H2MeshSteinerPoint*) point)->neighborsPairings);
        for (unsigned int i=0; i<neighborIndices.size(); ++i)
        {
            res[i] = isometries[i]*res[i];
        }
    }

    return res;
}

IsomH2Representation H2Mesh::getRepresentation() const
{
    return rho;
}

int H2Mesh::nbPoints() const
{
    return (int) meshPoints.size();
}
