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

H2MeshFunction::H2MeshFunction(const H2Mesh *mesh, const IsomH2Representation &rhoImage) : mesh(mesh), rhoImage(rhoImage)
{
}

void swap(H2MeshFunction &first, H2MeshFunction &second)
{
    std::swap(first.mesh, second.mesh);
    std::swap(first.rhoImage, second.rhoImage);
    std::swap(first.values, second.values);
    std::swap(first.functionSubdivisions, second.functionSubdivisions);
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
    std::vector<Word> vertexPairings = rhoDomain.getGroup()->getPairingsClosedSurfaceFromVertex();

    for (const auto & pairing : vertexPairings)
    {
        vertexImages.push_back(rhoImage.evaluateRepresentation(pairing)*basePoint);
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

    refreshSubdivisions();
}

void H2MeshFunction::initializePLsmart()
{
    H2Point basept = rhoImage.generatePolygon(100).getVertex(0);
    initializePL(basept);
}

H2MeshFunction::H2MeshFunction(const H2Mesh *mesh, const IsomH2Representation &rhoImage, const std::vector<H2Point> &values) :
     mesh(mesh), rhoImage(rhoImage), values(values)
{
    refreshSubdivisions();
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

std::vector<H2Point> H2MeshFunction::getPoints() const
{
    return values;
}

IsomH2Representation H2MeshFunction::getTargetRepresentation() const
{
    return rhoImage;
}

std::vector<H2GeodesicArc> H2MeshFunction::getExteriorSides() const
{
    std::vector<H2Point> vertices;
    std::vector<int> indices = mesh->exteriorSidesIndices;
    vertices.reserve(indices.size());
    for (auto index : indices)
    {
        vertices.push_back(values.at(index));
    }
    return H2Polygon(vertices).getSides();
}

std::vector<H2Triangle> H2MeshFunction::getTrianglesUp() const
{
    std::vector<H2Triangle> output;
    int aIndex, bIndex, cIndex;
    int L = H2TriangleSubdivision::nbOfLines(mesh->depth);
    int i, j, m = 0;
    output.reserve((mesh->subdivisions.size()*(L-1)*L)/2);


    for (const auto & meshIndices : mesh->meshIndicesInSubdivisions)
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
                output.push_back(H2Triangle(values[aIndex], values[bIndex], values[cIndex]));
            }
        }
    }

    return output;
}

H2Point H2MeshFunction::getH2Point(int index) const
{
    return values.at(index);
}

std::vector<H2Point> H2MeshFunction::getH2Neighbors(int index) const
{
    std::vector<int> neighborsIndices = mesh->meshPoints.at(index)->neighborsIndices;
    std::vector<H2Point> out;
    out.reserve(neighborsIndices.size());
    for (auto k : neighborsIndices)
    {
        out.push_back(values[k]);
    }
    return out;
}

std::vector<H2Point> H2MeshFunction::getKickedH2Neighbors(int index) const
{
    H2MeshPoint* point = mesh->meshPoints.at(index);
    std::vector<int> neighborIndices = point->neighborsIndices;
    std::vector<H2Point> res = getH2Neighbors(index);

    if (point->isBoundaryPoint())
    {
        std::vector<H2Isometry> isometries = rhoImage.evaluateRepresentation(((H2MeshBoundaryPoint*) point)->neighborsPairings);
        for (unsigned int i=0; i<neighborIndices.size(); ++i)
        {
            res[i] = isometries[i]*res[i];
        }
    }
    else if (point->isVertexPoint())
    {
        std::vector<H2Isometry> isometries = rhoImage.evaluateRepresentation(((H2MeshVertexPoint*) point)->neighborsPairings);
        for (unsigned int i=0; i<neighborIndices.size(); ++i)
        {
            res[i] = isometries[i]*res[i];
        }
    }
    else if (point->isSteinerPoint())
    {
        std::vector<H2Isometry> isometries = rhoImage.evaluateRepresentation(((H2MeshSteinerPoint*) point)->neighborsPairings);
        for (unsigned int i=0; i<neighborIndices.size(); ++i)
        {
            res[i] = isometries[i]*res[i];
        }
    }

    return res;
}

std::vector<H2Point> H2MeshFunction::getPartnerPoints(int index) const
{
    H2MeshPoint *point = mesh->meshPoints.at(index);
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
        return getFundamentalDomain().getVertices();
    }
    else
    {
        return {getH2Point(index)};
    }
}

bool H2MeshFunction::triangleContaining(const H2Point &point, H2Triangle &outputTriangle, int &meshIndex1, int &meshIndex2, int &meshIndex3) const
{
    int indexInSubdivision1, indexInSubdivision2, indexInSubdivision3, i=0;
    for(const auto &S : functionSubdivisions)
    {
        if (S.triangleContaining(point, outputTriangle, indexInSubdivision1, indexInSubdivision2, indexInSubdivision3))
        {
            meshIndex1 = mesh->meshIndicesInSubdivisions[i][indexInSubdivision1];
            meshIndex2 = mesh->meshIndicesInSubdivisions[i][indexInSubdivision2];
            meshIndex3 = mesh->meshIndicesInSubdivisions[i][indexInSubdivision3];
            return true;
        }
        ++i;
    }
    return false;
}

bool H2MeshFunction::isInteriorPoint(int meshIndex) const
{
    return mesh->isInteriorPoint(meshIndex);
}

H2Polygon H2MeshFunction::getFundamentalDomain() const
{
    std::vector<H2Point> vertices;
    vertices.reserve(mesh->vertexPoints.size());
    for (const auto & point : mesh->vertexPoints)
    {
        vertices.push_back(getH2Point(point.index));
    }
    return H2Polygon(vertices);
}

H2Triangle H2MeshFunction::getH2Triangle(int index1, int index2, int index3) const
{
    return H2Triangle(values.at(index1), values.at(index2), values.at(index3));
}

void H2MeshFunction::refreshSubdivisions()
{
    std::vector<std::vector<int>> meshIndicesInSubdivisions = mesh->meshIndicesInSubdivisions;
    std::vector<H2Point> points;
    int depth = mesh->depth;
    functionSubdivisions.clear();
    functionSubdivisions.reserve(meshIndicesInSubdivisions.size());

    for (const auto & meshIndices : meshIndicesInSubdivisions)
    {
        points.clear();
        points.reserve(meshIndices.size());
        for (auto index : meshIndices)
        {
            points.push_back(values.at(index));
        }
        functionSubdivisions.push_back(H2TriangleSubdivision(points, depth));
    }
}

IsomH2Representation H2MeshFunction::getRepresentation() const
{
    return rhoImage;
}
