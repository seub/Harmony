#include "h2mesh.h"

#include "h2isometry.h"
#include "h2trianglesubdivision.h"
#include "h2meshconstructor.h"


H2Mesh::H2Mesh(const IsomH2Representation &rho, int depth) : rho(rho), depth(depth)
{
    fundamentalDomain = rho.generatePolygon(30);
    H2MeshConstructor(this);
}

H2Mesh::H2Mesh(const H2Mesh &other)
{
    rho = other.rho;
    depth = other.depth;

    fundamentalDomain = other.fundamentalDomain;
    fundamentalSteinerDomain = other.fundamentalSteinerDomain;
    exteriorSidesIndices = other.exteriorSidesIndices;

    subdivisions = other.subdivisions;
    meshIndicesInSubdivisions = other.meshIndicesInSubdivisions;
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
    std::swap(first.exteriorSidesIndices, second.exteriorSidesIndices);

    std::swap(first.subdivisions, second.subdivisions);
    std::swap(first.meshIndicesInSubdivisions, second.meshIndicesInSubdivisions);
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
    int indexInSubdivision1, indexInSubdivision2, indexInSubdivision3, i=0;
    for(const auto &S : subdivisions)
    {
        if (S.triangleContaining(point, outputTriangle, indexInSubdivision1, indexInSubdivision2, indexInSubdivision3))
        {
            meshIndex1 = meshIndicesInSubdivisions[i][indexInSubdivision1];
            meshIndex2 = meshIndicesInSubdivisions[i][indexInSubdivision2];
            meshIndex3 = meshIndicesInSubdivisions[i][indexInSubdivision3];
            return true;
        }
        ++i;
    }
    return false;
}

H2Point H2Mesh::getH2Point(int index) const
{
    return subdivisions[meshPoints[index]->subdivisionIndex].points->at(meshPoints[index]->indexInSubdivision);
}

H2Triangle H2Mesh::getH2Triangle(int index1, int index2, int index3) const
{
    return H2Triangle(getH2Point(index1), getH2Point(index2), getH2Point(index3));
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

std::vector<H2Point> H2Mesh::getPartnerPoints(int index) const
{
    H2MeshPoint *point = meshPoints.at(index);
    if (point->isBoundaryPoint())
    {
        return {getH2Point(index), getH2Point(((H2MeshBoundaryPoint *) point)->partnerPointIndex)};
    }
    else if (point->isSteinerPoint())
    {
        return {getH2Point(index), getH2Point(((H2MeshSteinerPoint *) point)->partnerPointIndex)};
    }
    else if (point->isVertexPoint())
    {
        return fundamentalDomain.getVertices();
    }
    else
    {
        return {getH2Point(index)};
    }
}

std::vector<H2Point> H2Mesh::getPoints() const
{
    std::vector<H2Point> out;
    out.reserve(meshPoints.size());
    for (std::vector<H2MeshPoint*>::size_type i=0; i != meshPoints.size(); ++i)
    {
        out.push_back(getH2Point(i));
    }
    return out;
}

std::vector<H2GeodesicArc> H2Mesh::getSides() const
{
    return fundamentalDomain.getSides();
}

std::vector<H2Triangle> H2Mesh::getTrianglesUp() const
{
    std::vector<H2Triangle> output;
    int aIndex, bIndex, cIndex;
    int L = H2TriangleSubdivision::nbOfLines(depth);
    int i, j, m = 0;
    output.reserve((subdivisions.size()*(L-1)*L)/2);


    for (const auto & meshIndices : meshIndicesInSubdivisions)
    {
        m = 0;
        for (i=0; i<L - 1; i++)
        {
            m += i;
            for (j=0; j<=i; j++)
            {
                aIndex = meshIndices.at(m + j);
                bIndex = meshIndices.at(m + j + i + 1);
                cIndex = meshIndices.at(m + j + i + 2);
                output.push_back(H2Triangle(getH2Point(aIndex), getH2Point(bIndex), getH2Point(cIndex)));
            }
        }
    }

    return output;
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

bool H2Mesh::isInteriorPoint(int index) const
{
    return meshPoints.at(index)->isInteriorPoint();
}
