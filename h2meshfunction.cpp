#include "h2meshfunction.h"
#include "h2polygontriangulater.h"
#include "triangularsubdivision.h"
#include "h2meshfunctioniterator.h"

H2MeshFunction::H2MeshFunction() : mesh(nullptr)
{
}

H2MeshFunction::H2MeshFunction(const H2Mesh *mesh, const GroupRepresentation<H2Isometry> &rhoImage) : mesh(mesh), rhoImage(rhoImage)
{
}


void H2MeshFunction::initializePL(const H2Point &basePoint)
{
    std::vector<H2Point> vertexImages;
    uint nbVertices = mesh->fundamentalDomain.nbVertices();
    vertexImages.reserve(nbVertices);
    GroupRepresentation<H2Isometry> rhoDomain = mesh->getRepresentation();
    std::vector<Word> vertexPairings = rhoDomain.getDiscreteGroup().getPairingsClosedSurfaceFromVertex();

    for (const auto & pairing : vertexPairings)
    {
        vertexImages.push_back(rhoImage.evaluateRepresentation(pairing)*basePoint);
    }

    H2SteinerPolygon SteinerPolygonImage(vertexImages, mesh->fundamentalSteinerDomain.getVectorNbSteinerPoints());

    std::vector<H2Point> SteinerPolygonImageVertices = SteinerPolygonImage.getFullPolygon().getVertices();


    std::vector<TriangulationTriangle> triangles = mesh->triangles;
    std::vector< TriangularSubdivision<H2Point> > subdivisionImages;
    subdivisionImages.reserve(triangles.size());
    uint i,j,k, depth=mesh->depth;

    TriangularSubdivision<H2Point> TS(depth);
    for(const auto & triangle : triangles)
    {
        triangle.getVertices(i,j,k);
        TS.initializeSubdivisionByMidpoints(SteinerPolygonImageVertices[i],SteinerPolygonImageVertices[j],SteinerPolygonImageVertices[k]);
        subdivisionImages.push_back(TS);
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
    H2Point basept = rhoImage.generateFundamentalDomain().getVertex(0);
    initializePL(basept);
}

H2MeshFunction::H2MeshFunction(const H2Mesh *mesh, const GroupRepresentation<H2Isometry> &rhoImage, const std::vector<H2Point> &values) :
     mesh(mesh), rhoImage(rhoImage), values(values)
{
    refreshSubdivisions();
}

void H2MeshFunction::iterate(uint n)
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

GroupRepresentation<H2Isometry> H2MeshFunction::getTargetRepresentation() const
{
    return rhoImage;
}

std::vector<H2GeodesicArc> H2MeshFunction::getExteriorSides() const
{
    std::vector<H2Point> vertices;
    std::vector<uint> indices = mesh->exteriorSidesIndices;
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
    uint aIndex, bIndex, cIndex;
    uint L = TriangularSubdivision<H2Point>::nbLines(mesh->depth);
    uint i, j, m = 0;
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

H2Point H2MeshFunction::getH2Point(uint index) const
{
    return values.at(index);
}

std::vector<H2Point> H2MeshFunction::getH2Neighbors(uint index) const
{
    std::vector<uint> neighborsIndices = mesh->meshPoints.at(index)->neighborsIndices;
    std::vector<H2Point> out;
    out.reserve(neighborsIndices.size());
    for (auto k : neighborsIndices)
    {
        out.push_back(values[k]);
    }
    return out;
}

std::vector<H2Point> H2MeshFunction::getKickedH2Neighbors(uint index) const
{
    H2MeshPoint* point = mesh->meshPoints.at(index);
    std::vector<uint> neighborIndices = point->neighborsIndices;
    std::vector<H2Point> res = getH2Neighbors(index);

    if (point->isBoundaryPoint())
    {
        std::vector<H2Isometry> isometries = rhoImage.evaluateRepresentation((static_cast<H2MeshBoundaryPoint*>(point))->neighborsPairings);
        for (uint i=0; i!=neighborIndices.size(); ++i)
        {
            res[i] = isometries[i]*res[i];
        }
    }
    else if (point->isVertexPoint())
    {
        std::vector<H2Isometry> isometries = rhoImage.evaluateRepresentation((static_cast<H2MeshVertexPoint*>(point))->neighborsPairings);
        for (uint i=0; i!=neighborIndices.size(); ++i)
        {
            res[i] = isometries[i]*res[i];
        }
    }
    else if (point->isSteinerPoint())
    {
        std::vector<H2Isometry> isometries = rhoImage.evaluateRepresentation((static_cast<H2MeshSteinerPoint*>(point))->neighborsPairings);
        for (uint i=0; i!=neighborIndices.size(); ++i)
        {
            res[i] = isometries[i]*res[i];
        }
    }

    return res;
}

std::vector<H2Point> H2MeshFunction::getPartnerPoints(uint index) const
{
    H2MeshPoint *point = mesh->meshPoints.at(index);
    if (point->isBoundaryPoint())
    {
        return {getH2Point(index), getH2Point((static_cast<H2MeshBoundaryPoint*>(point))->partnerPointIndex)};
    }
    else if (point->isSteinerPoint())
    {
        return {getH2Point(index), getH2Point((static_cast<H2MeshSteinerPoint*>(point))->partnerPointIndex)};
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

bool H2MeshFunction::triangleContaining(const H2Point &point, H2Triangle &outputTriangle, uint &meshIndex1, uint &meshIndex2, uint &meshIndex3) const
{
    H2Point A, B, C;
    uint indexInSubdivision1, indexInSubdivision2, indexInSubdivision3, i=0;
    for(const auto &S : functionSubdivisions)
    {
        if (S.triangleContaining(point, A, B, C, indexInSubdivision1, indexInSubdivision2, indexInSubdivision3))
        {
            meshIndex1 = mesh->meshIndicesInSubdivisions[i][indexInSubdivision1];
            meshIndex2 = mesh->meshIndicesInSubdivisions[i][indexInSubdivision2];
            meshIndex3 = mesh->meshIndicesInSubdivisions[i][indexInSubdivision3];
            outputTriangle = H2Triangle(A, B, C);
            return true;
        }
        ++i;
    }
    return false;
}

bool H2MeshFunction::isInteriorPoint(uint meshIndex) const
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

H2Triangle H2MeshFunction::getH2Triangle(uint index1, uint index2, uint index3) const
{
    return H2Triangle(values.at(index1), values.at(index2), values.at(index3));
}

void H2MeshFunction::refreshSubdivisions()
{
    std::vector< std::vector<uint> > meshIndicesInSubdivisions = mesh->meshIndicesInSubdivisions;
    std::vector<H2Point> points;
    uint depth = mesh->depth;
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
        functionSubdivisions.push_back(TriangularSubdivision<H2Point>(points, depth));
    }
}
