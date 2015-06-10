#include "h2meshfunction.h"
#include "h2polygontriangulater.h"
#include "h2trianglesubdivision.h"
#include "h2meshfunctioniterator.h"

H2MeshFunction::H2MeshFunction(const H2Mesh *const mesh, const IsomH2Representation &rhoImage) : mesh(mesh), rhoImage(rhoImage)
{
}

void H2MeshFunction::initializePL(const H2Point &basePoint)
{
    std::vector<H2Point> vertexImages;
    int nbVertices = mesh->fundamentalDomain.nbVertices();
    vertexImages.reserve(nbVertices);
    IsomH2Representation rhoDomain = mesh->getRepresentation();
    std::vector<Word> vertexPairings = rhoDomain.getVertexPairings();

    for (const auto & pairing : vertexPairings)
    {
        vertexImages.push_back(rhoImage.evaluateRepresentation(pairing)*basePoint);
    }

    std::vector<TriangulationTriangle> triangles = mesh->triangles;
    std::vector<H2TriangleSubdivision> subdivisionImages;
    subdivisionImages.reserve(triangles.size());

    int i,j,k, depth=mesh->depth;
    for(const auto & triangle : triangles)
    {
        triangle.getVertices(i,j,k);
        subdivisionImages.push_back(H2TriangleSubdivision(vertexImages[i],vertexImages[j],vertexImages[k],depth));
    }

    int l=0;
    for(const auto & meshpoint : mesh->meshPoints)
    {
        values[l]= subdivisionImages[meshpoint->subdivisionIndex].getPoint(meshpoint->indexInSubdivision);
        ++l;
    }
}

H2MeshFunction::H2MeshFunction(const H2Mesh * const mesh, const IsomH2Representation &rhoImage, std::vector<H2Point> values) :
     mesh(mesh), rhoImage(rhoImage), values(values)
{
}

void H2MeshFunction::iterate(int n)
{
    H2MeshFunctionIterator iter(this);
    iter.iterate(n);
    values = iter.getOutput().values;
}
