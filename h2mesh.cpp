#include "h2mesh.h"

#include "h2isometry.h"
#include "h2trianglesubdivision.h"
#include "h2meshconstructor.h"


H2Mesh::H2Mesh()
{

}

H2MeshPoint::H2MeshPoint(int subdivisionIndex, int indexInSubdivision, bool cutPoint, int subdivisionIndexRight,
                         int indexInSubdivisionRight, bool boundaryPoint, int side, bool vertexPoint, int vertexIndex) :
    subdivisionIndex(subdivisionIndex), indexInSubdivision(indexInSubdivision), cutPoint(cutPoint), subdivisionIndexRight(subdivisionIndexRight),
    indexInSubdivisionRight(indexInSubdivisionRight), boundaryPoint(boundaryPoint), side(side), vertexPoint(vertexPoint), vertexIndex(vertexIndex)
{

}


H2Mesh::H2Mesh(const IsomH2Representation &rho, int depth) : rho(rho), depth(depth)
{
    clock_t start = clock();
    fundamentalDomain = rho.generatePolygon(100);
    clock_t end = clock();
    std::cout << "Time to generate fundamental domain: " << (end - start)*1.0/CLOCKS_PER_SEC << "s" << std::endl;

    H2MeshConstructor(this);
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
    return subdivisions[meshPoints[index].subdivisionIndex].points->at(meshPoints[index].indexInSubdivision);
}

std::vector<H2Point> H2Mesh::getNeighbors(int index) const
{
    std::vector<int> neighborIndices = meshPoints.at(index).neighborsIndices;
    std::vector<H2Point> res;
    for (auto k : neighborIndices)
    {
        res.push_back(getH2Point(k));
    }
    return res;
}
