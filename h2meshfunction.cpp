#include "h2meshfunction.h"
#include "h2polygontriangulater.h"
#include "h2trianglesubdivision.h"
#include "h2meshfunctioniterator.h"

H2MeshFunction::H2MeshFunction() : mesh(0), rhoImage(0)
{
}

H2MeshFunction::H2MeshFunction(const H2MeshFunction &other) : H2MeshFunction(other.mesh, other.rhoImage, other.values)
{
}

H2MeshFunction::H2MeshFunction(const H2Mesh *mesh, const IsomH2Representation *rhoImage) : mesh(mesh), rhoImage(rhoImage)
{
}

void swap(H2MeshFunction &first, H2MeshFunction &second)
{
    std::swap(first.mesh, second.mesh);
    std::swap(first.rhoImage, second.rhoImage);
    std::swap(first.values, second.values);
}

H2MeshFunction & H2MeshFunction::operator=(H2MeshFunction other)
{
    swap(*this, other);
    return *this;
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
        vertexImages.push_back(rhoImage->evaluateRepresentation(pairing)*basePoint);
    }

    H2SteinerPolygon SteinerPolygonImage(vertexImages, mesh->fundamentalSteinerDomain.getVectorNbSteinerPoints());

    std::vector<H2Point> SteinerPolygonImageVertices = SteinerPolygonImage.getFullPolygon().getVertices();


    std::vector<TriangulationTriangle> triangles = mesh->triangles;
    std::vector<H2TriangleSubdivision> subdivisionImages;
    subdivisionImages.reserve(triangles.size());
    int i,j,k, depth=mesh->depth;
    for(const auto & triangle : triangles)
    {
        triangle.getVertices(i,j,k);
        subdivisionImages.push_back(H2TriangleSubdivision(SteinerPolygonImageVertices[i],SteinerPolygonImageVertices[j],SteinerPolygonImageVertices[k],depth));
    }

    values.clear();
    values.reserve(mesh->meshPoints.size());
    for(const auto & meshpoint : mesh->meshPoints)
    {
        values.push_back(subdivisionImages[meshpoint->subdivisionIndex].getPoint(meshpoint->indexInSubdivision));
    }
}

void H2MeshFunction::initializePLsmart()
{
    H2Point basept = rhoImage->generatePolygon(100).vertices[0];
    initializePL(basept);
}

H2MeshFunction::H2MeshFunction(const H2Mesh *mesh, const IsomH2Representation *rhoImage, const std::vector<H2Point> &values) :
     mesh(mesh), rhoImage(rhoImage), values(values)
{
}

void H2MeshFunction::iterate(int n)
{
    H2MeshFunctionIterator iter(this);
    iter.iterate(n);
    iter.getOutput(*this);
}

const std::vector<H2Point> & H2MeshFunction::getValues() const
{
    return values;
}

std::vector<H2Triangle> H2MeshFunction::getTriangles() const
{
    std::vector<H2Triangle> output;
    H2Point a, b, c;
    int aIndex, bIndex, cIndex;
    int L = mesh->subdivisions.front().nbOfLines(mesh->depth);
    int i, j, m = 0;
    output.reserve((mesh->subdivisions.size()*(L-1)*L)/2);


    for (const auto & S : mesh->subdivisions)
    {
        m = 0;
        for (i=0; i<L - 1; i++)
        {
            m += i;
            for (j=0; j<=i; j++)
            {
                aIndex = S.meshIndices->at(m + j);
                bIndex = S.meshIndices->at(m + j + i + 1);
                cIndex = S.meshIndices->at(m + j + i + 2);
                output.push_back(H2Triangle(values[aIndex], values[bIndex], values[cIndex]));
            }
        }
    }

    return output;
}
