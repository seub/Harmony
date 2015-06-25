#include "h2buffer.h"

#include "h2mesh.h"
#include "h2meshfunction.h"
#include "liftedgraph.h"

H2Buffer::H2Buffer()
{
    isMeshEmpty = true;
    isFunctionEmpty = true;
    isRhoEmpty = true;
}

bool H2Buffer::getIsMeshEmpty() const
{
    return isMeshEmpty;
}

bool H2Buffer::getIsFunctionEmpty() const
{
    return isFunctionEmpty;
}

bool H2Buffer::getIsRhoEmpty() const
{
    return isRhoEmpty;
}

void H2Buffer::setIsFunctionEmpty(bool isFunctionEmpty)
{
    this->isFunctionEmpty = isFunctionEmpty;
}

void H2Buffer::setIsMeshEmpty(bool isMeshEmpty)
{
    this->isMeshEmpty = isMeshEmpty;
}

void H2Buffer::setIsRhoEmpty(bool isRhoEmpty)
{
    this->isRhoEmpty = isRhoEmpty;
}

void H2Buffer::setMeshPointer(const H2Mesh *mesh, const QColor &color)
{
    this->mesh = mesh;
    meshOrFunctionColor = color;
}

void H2Buffer::setFunctionPointer(const H2MeshFunction *function, const QColor &color)
{
    this->function = function;
    meshOrFunctionColor = color;
}

void H2Buffer::setRhoPointer(const GroupRepresentation<H2Isometry> *rho, const QColor &color)
{
    this->rho = rho;
    rhoColor = color;
}

void H2Buffer::refreshMesh()
{
    if (isMeshEmpty)
    {
        throw(QString("Error in H2Buffer::refreshMesh(): empty mesh?"));
    }

    meshOrFunctionSides = mesh->getSides();

    std::vector<H2Triangle> triangles = mesh->getTrianglesUp();
    meshOrFunctionArcs.clear();
    meshOrFunctionArcs.reserve(3*triangles.size());
    std::vector<H2GeodesicArc> sides;
    for (const auto & triangle : triangles)
    {
        sides = triangle.getSides();
        meshOrFunctionArcs.insert(meshOrFunctionArcs.end(), sides.begin(), sides.end());
    }
}

void H2Buffer::refreshFunction()
{
    if (isFunctionEmpty)
    {
        throw(QString("Error in H2Buffer::refreshFunction(): empty function?"));
    }


    meshOrFunctionSides = function->getExteriorSides();


    std::vector<H2Triangle> triangles = function->getTrianglesUp();
    meshOrFunctionArcs.clear();
    meshOrFunctionArcs.reserve(3*triangles.size());
    std::vector<H2GeodesicArc> sides;
    for (const auto & triangle : triangles)
    {
        sides = triangle.getSides();
        meshOrFunctionArcs.insert(meshOrFunctionArcs.end(), sides.begin(), sides.end());
    }
}

void H2Buffer::refreshMeshOrFunctionSideTranslates()
{
    if (isMeshEmpty && isFunctionEmpty)
    {
        throw(QString("Error in H2Buffer::addMeshOrFunctionSideTranslates: mesh/function empty?"));
    }
    std::vector<H2GeodesicArc> sideTranslates;
    meshOrFunctionSidesTranslatesAroundVertices.clear();
    meshOrFunctionSidesTranslatesAroundVertices.reserve(translationsAroundVertices.size()*meshOrFunctionSides.size());

    for (const auto & translation : translationsAroundVertices)
    {
        sideTranslates = translation*meshOrFunctionSides;
        meshOrFunctionSidesTranslatesAroundVertices.insert(meshOrFunctionSidesTranslatesAroundVertices.end(), sideTranslates.begin(), sideTranslates.end());
    }
}

void H2Buffer::refreshMeshOrFunctionTranslates(bool aroundVertex, bool aroundVertices)
{
    if (aroundVertex && aroundVertices)
    {
        throw(QString("Error in H2Buffer::addMeshTranslates: both booleans are true"));
    }

    refreshMeshOrFunctionSideTranslates();

    if (aroundVertex)
    {
        refreshMeshOrFunctionArcsTranslatesAroundVertex();
    }
    if (aroundVertices)
    {
        refreshMeshOrFunctionArcsTranslatesAroundVertices();
    }
}

void H2Buffer::refreshMeshOrFunctionArcsTranslatesAroundVertices()
{
    if (isMeshEmpty && isFunctionEmpty)
    {
        throw(QString("Error in H2Buffer::addMeshOrFunctionTranslatesAroundVertices: mesh/function empty?"));
    }

    std::vector<H2GeodesicArc> arcsTranslates;

    meshOrFunctionArcsTranslatesAroundVertices.clear();
    meshOrFunctionArcsTranslatesAroundVertices.reserve(translationsAroundVertices.size()*meshOrFunctionArcs.size());

    for (const auto & A : translationsAroundVertices)
    {
        arcsTranslates = A*meshOrFunctionArcs;
        meshOrFunctionArcsTranslatesAroundVertices.insert(meshOrFunctionArcsTranslatesAroundVertices.end(), arcsTranslates.begin(), arcsTranslates.end());
    }
}

void H2Buffer::refreshMeshOrFunctionArcsTranslatesAroundVertex()
{
    if (isMeshEmpty && isFunctionEmpty)
    {
        throw(QString("Error in H2Buffer::addMeshOrFunctionTranslatesAroundVertex: mesh/function empty?"));
    }

    std::vector<H2GeodesicArc> arcsTranslates;
    meshOrFunctionArcsTranslatesAroundVertex.clear();
    meshOrFunctionArcsTranslatesAroundVertex.reserve(translationsAroundVertex.size()*meshOrFunctionArcs.size());

    for (const auto & A : translationsAroundVertex)
    {
        arcsTranslates = A*meshOrFunctionArcs;
        meshOrFunctionArcsTranslatesAroundVertex.insert(meshOrFunctionArcsTranslatesAroundVertex.end(), arcsTranslates.begin(), arcsTranslates.end());
    }
}


void H2Buffer::refreshRho()
{
    if (isRhoEmpty)
    {
        throw(QString("Error in H2Buffer::refreshRho: rho empty?"));
    }

    H2Geodesic L;
    std::vector<H2Isometry> generatorImages = rho->getGeneratorImages();
    rhoAxes.clear();
    for (const auto &f : generatorImages)
    {
        if (f.axis(L))
        {
            rhoAxes.push_back(L);
        }
        else
        {
            qDebug() << "WARNING in H2Buffer::refreshRho(): some generators are not hyperbolic";
        }
    }

    translationsAroundVertex = rho->getSidePairingsNormalizedAroundVertex();
    translationsAroundVertices = rho->getSidePairingsNormalizedAroundVertices();
}
