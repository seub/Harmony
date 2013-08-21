#include "h2mesh.h"

#include "h2isometry.h"
#include "h2trianglesubdivision.h"
#include "h2meshconstructor.h"


H2MeshPoint::H2MeshPoint(int subdivisionIndex, int indexInSubdivision, bool cutPoint, int subdivisionIndexRight,
                         int indexInSubdivisionRight, bool boundaryPoint, int side, bool vertexPoint, int vertexIndex) :
    subdivisionIndex(subdivisionIndex), indexInSubdivision(indexInSubdivision), cutPoint(cutPoint), subdivisionIndexRight(subdivisionIndexRight),
    indexInSubdivisionRight(indexInSubdivisionRight), boundaryPoint(boundaryPoint), side(side), vertexPoint(vertexPoint), vertexIndex(vertexIndex)
{

}

H2Mesh::H2Mesh(const IsomH2Representation &rho, int depth) : rho(rho), depth(depth)
{
    fundamentalDomain = rho.generatePolygon(200);
    H2MeshConstructor(this);
}
