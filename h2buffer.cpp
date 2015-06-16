#include "h2buffer.h"

H2Buffer::H2Buffer()
{
    isMeshEmpty = true;
    isMeshFunctionEmpty = true;
}

void H2Buffer::addElement(const H2Point &point, const QColor &color, int width)
{
    points.push_back(point);
    pointsColors.push_back(color);
    pointsWidths.push_back(width);
    return;
}

void H2Buffer::addElement(const H2Geodesic &geodesic, const QColor &color, int width)
{
    geodesics.push_back(geodesic);
    geodesicsColors.push_back(color);
    geodesicsWidths.push_back(width);
    return;
}

void H2Buffer::addElement(const H2GeodesicArc &geodesicArc, const QColor &color, int width)
{
    geodesicArcs.push_back(geodesicArc);
    geodesicArcsColors.push_back(color);
    geodesicArcsWidths.push_back(width);
    return;
}

void H2Buffer::addElement(const std::vector<H2GeodesicArc> &geodesicArcs, const QColor & color, int width)
{
    for (auto arc : geodesicArcs)
    {
        addElement(arc, color, width);
    }
}

void H2Buffer::addElement(const H2Polygon &polygon, const QColor &color, int width)
{
    addElement(polygon.getSides(), color, 1);
    addElement(polygon.getVertices(), "black", width);
}

void H2Buffer::addElement(const H2Triangle &triangle, const QColor &color, int width)
{
    addElement(triangle.getPoints(), "black", width);
    addElement(triangle.getSides(), color, 1);
}

void H2Buffer::addElement(const std::vector<H2Triangle> &triangles, const QColor &color, int width)
{
    for (unsigned int j=0; j<triangles.size(); j++)
    {
        addElement(triangles[j], color, width);
    }
    return;
}

void H2Buffer::addElement(const H2Isometry &isometry, const QColor &color, int width)
{
    H2Geodesic L;
    if (isometry.axis(L))
    {
        geodesics.push_back(L);
        geodesicsColors.push_back(color);
        geodesicsWidths.push_back(width);
    }
    return;
}


void H2Buffer::addElement(const IsomH2Representation &rho, const QColor &color, int width)
{
    std::vector<H2Isometry> generatorImages = rho.getGeneratorImages();
    for (unsigned int i=0; i<generatorImages.size(); i++)
    {
        addElement(generatorImages[i], color, width);
    }
    return;
}

void H2Buffer::addElement(const std::vector<H2Isometry> &V, const QColor &color, int width)
{
    for (unsigned int i=0; i<V.size(); i++)
    {
        addElement(V[i], color, width);
    }
    return;
}

void H2Buffer::addElement(const std::vector<H2Polygon> &V, const QColor &color, int width)
{
    for (unsigned int i=0; i<V.size(); i++)
    {
        addElement(V[i], color, width);
    }
    return;
}

void H2Buffer::addElement(const H2TriangleSubdivision &T, const QColor &color, int width)
{
    if (T.isRoot())
    {
        int L = T.nbOfLines(T.getTotalDepth());
        std::vector<H2Point> Tpoints = T.getPoints();

        H2Point a, b, c;
        int i, j, m = 0;
        for (i=0; i<L - 1; i++)
        {
            m += i;
            for (j=0; j<=i; j++)
            {
                a = Tpoints[m + j];
                b = Tpoints[m + j + i + 1];
                c = Tpoints[m + j + i + 2];
                addElement(H2Triangle(a, b, c), color, width);
            }
        }
    }
    else
    {
        throw(QString("Error in  H2Buffer::addElement(const H2TriangleSubdivision &T, const QColor &color, int width): not a root subdivision"));
    }
}

void H2Buffer::addElement(const H2Mesh *mesh, const QColor &color, int width)
{
    this->mesh = mesh;
    isMeshEmpty = false;

    meshColor = color;
    meshWidth = width;
    refreshMesh();
}

void H2Buffer::addElement(H2MeshFunction *f, const QColor &color, int width)
{
    this->function = f;
    isMeshFunctionEmpty = false;

    meshColor = color;
    meshWidth = width;
    refreshFunction();
}

void H2Buffer::refreshMesh()
{
    meshSides = mesh->getSides();
    meshPoints = mesh->getPoints();

    std::vector<H2Triangle> triangles = mesh->getTrianglesUp();
    meshArcs.clear();
    meshArcs.reserve(3*triangles.size());
    std::vector<H2GeodesicArc> sides;
    for (const auto & triangle : triangles)
    {
        sides = triangle.getSides();
        meshArcs.insert(meshArcs.end(), sides.begin(), sides.end());
    }
}

void H2Buffer::refreshFunction()
{
    meshSides = function->getExteriorSides();
    meshPoints = function->getPoints();

    std::vector<H2Triangle> triangles = function->getTrianglesUp();
    meshArcs.clear();
    meshArcs.reserve(3*triangles.size());
    std::vector<H2GeodesicArc> sides;
    for (const auto & triangle : triangles)
    {
        sides = triangle.getSides();
        meshArcs.insert(meshArcs.end(), sides.begin(), sides.end());
    }
}

void H2Buffer::addElement(const std::vector<H2Point> & points, const QColor & color, int width)
{
    for (const auto & point : points)
    {
        addElement(point, color, width);
    }
}


void H2Buffer::addMeshTranslates(const QColor &color, int width)
{
    addMeshTranslates(translations, color, width);
}

void H2Buffer::addMeshTranslates(const std::vector<H2Isometry> &translations, const QColor &color, int width)
{
    meshTranslatesColor = color;
    meshTranslatesWidth = width;

    std::vector<H2Point> pointsTranslates;
    std::vector<H2GeodesicArc> arcsTranslates, sideTranslates;

    meshPointsTranslates.clear();
    meshPointsTranslates.reserve(translations.size()*meshPoints.size());
    meshArcsTranslates.clear();
    meshArcsTranslates.reserve(translations.size()*meshArcs.size());
    meshSidesTranslates.clear();
    meshSidesTranslates.reserve(translations.size()*meshSides.size());

    for (const auto & A : translations)
    {
        pointsTranslates = A*meshPoints;
        meshPointsTranslates.insert(meshPointsTranslates.end(), pointsTranslates.begin(), pointsTranslates.end());

        arcsTranslates = A*meshArcs;
        meshArcsTranslates.insert(meshArcsTranslates.end(), arcsTranslates.begin(), arcsTranslates.end());

        sideTranslates = A*meshSides;
        meshSidesTranslates.insert(meshSidesTranslates.end(), sideTranslates.begin(), sideTranslates.end());
    }
}

void H2Buffer::setTranslations(const std::vector<H2Isometry> &isometries)
{
    this->translations = isometries;
}
