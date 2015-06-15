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

    const std::vector<H2TriangleSubdivision> & subdivisions = mesh->getSubdivisions();
    for (const auto &S : subdivisions)
    {
        addElement(S, color, width);
    }
}

void H2Buffer::addElement(H2MeshFunction *f, const QColor &color, int width)
{
    this->function = f;
    isMeshFunctionEmpty = false;
    //functionColor = color;
    //functionWidth = width;
    refreshFunction();
    functionColors.resize(functionPoints.size());
    functionWidths.resize(functionPoints.size());
    std::fill(functionColors.begin(), functionColors.end(), color);
    std::fill(functionWidths.begin(), functionWidths.end(), width);
}

void H2Buffer::refreshFunction()
{
    functionArcs.clear();
    functionPoints.clear();
    functionColors.clear();
    functionWidths.clear();
    std::vector<H2Triangle> triangles = function->getTrianglesUp();
    functionArcs.reserve(3*triangles.size());
    functionPoints.reserve(3*triangles.size());
    std::vector<H2GeodesicArc> sides;
    std::vector<H2Point> vertices;
    for (const auto & triangle : triangles)
    {
        sides = triangle.getSides();
        vertices = triangle.getPoints();
        functionArcs.insert(functionArcs.end(), sides.begin(), sides.end());
        functionPoints.insert(functionPoints.end(), vertices.begin(), vertices.end());
    }
}

void H2Buffer::addElement(const std::vector<H2Point> & points, const QColor & color, int width)
{
    for (const auto & point : points)
    {
        addElement(point, color, width);
    }
}

void H2Buffer::addKickedDrawing(const H2Isometry &A)
{
    std::vector<H2Point> tempPts = A*points;
    std::vector<int> tempWidths;
    std::vector<QColor> tempColors;
    tempColors.resize(points.size());
    tempWidths = pointsWidths;
    std::fill(tempColors.begin(), tempColors.end(), "grey");
    points.insert(points.end(),tempPts.begin(),tempPts.end());
    pointsWidths.insert(pointsWidths.end(),tempWidths.begin(),tempWidths.end());
    pointsColors.insert(pointsColors.end(), tempColors.begin(), tempColors.end());

    std::vector<H2GeodesicArc> tempGeodesicArcs = A*geodesicArcs;
    tempColors.resize(geodesicArcs.size());
    tempWidths = geodesicArcsWidths;
    std::fill(tempColors.begin(), tempColors.end(), "grey");
    geodesicArcs.insert(geodesicArcs.end(),tempGeodesicArcs.begin(),tempGeodesicArcs.end());
    geodesicArcsWidths.insert(geodesicArcsWidths.end(),tempWidths.begin(),tempWidths.end());
    geodesicArcsColors.insert(geodesicArcsColors.end(), tempColors.begin(), tempColors.end());

    std::vector<H2Point> tempFunctionPoints = A*functionPoints;
    tempColors.resize(functionPoints.size());
    tempWidths = functionWidths;
    std::fill(tempColors.begin(), tempColors.end(), "grey");
    functionPoints.insert(functionPoints.end(),tempFunctionPoints.begin(),tempFunctionPoints.end());
    functionWidths.insert(functionWidths.end(),tempWidths.begin(),tempWidths.end());
    functionColors.insert(functionColors.end(), tempColors.begin(), tempColors.end());

    std::vector<H2GeodesicArc> tempFunctionArcs = A*functionArcs;
    tempColors.resize(functionArcs.size());
    tempWidths = functionWidths;
    std::fill(tempColors.begin(), tempColors.end(), "grey");
    functionArcs.insert(functionArcs.end(),tempFunctionArcs.begin(),tempFunctionArcs.end());
    functionWidths.insert(functionWidths.end(),tempWidths.begin(),tempWidths.end());
    functionColors.insert(functionColors.end(), tempColors.begin(), tempColors.end());
}

void H2Buffer::addKickedDrawing(const std::vector<H2Isometry> &vectorA)
{
    std::vector<H2Point> tempPts, tempFunctionPoints, pointsHolder, functionPointsHolder;
    std::vector<int> tempWidths;
    std::vector<QColor> tempColors;
    std::vector<H2GeodesicArc> tempGeodesicArcs, tempFunctionArcs, arcsHolder, functionArcsHolder;

    pointsHolder = points;
    arcsHolder = geodesicArcs;
    functionPointsHolder = functionPoints;
    functionArcsHolder = functionArcs;

    for (const auto & A : vectorA)
    {
        tempPts = A*pointsHolder;
        tempColors.resize(pointsHolder.size());
        tempWidths = pointsWidths;
        std::fill(tempColors.begin(), tempColors.end(), "grey");
        points.insert(points.end(),tempPts.begin(),tempPts.end());
        pointsWidths.insert(pointsWidths.end(),tempWidths.begin(),tempWidths.end());
        pointsColors.insert(pointsColors.end(), tempColors.begin(), tempColors.end());

        tempGeodesicArcs = A*arcsHolder;
        tempColors.resize(geodesicArcs.size());
        tempWidths = geodesicArcsWidths;
        std::fill(tempColors.begin(), tempColors.end(), "grey");
        geodesicArcs.insert(geodesicArcs.end(),tempGeodesicArcs.begin(),tempGeodesicArcs.end());
        geodesicArcsWidths.insert(geodesicArcsWidths.end(),tempWidths.begin(),tempWidths.end());
        geodesicArcsColors.insert(geodesicArcsColors.end(), tempColors.begin(), tempColors.end());

        tempFunctionPoints = A*functionPointsHolder;
        tempColors.resize(functionPoints.size());
        tempWidths = functionWidths;
        std::fill(tempColors.begin(), tempColors.end(), "grey");
        functionPoints.insert(functionPoints.end(),tempFunctionPoints.begin(),tempFunctionPoints.end());
        functionWidths.insert(functionWidths.end(),tempWidths.begin(),tempWidths.end());
        functionColors.insert(functionColors.end(), tempColors.begin(), tempColors.end());

        tempFunctionArcs = A*functionArcsHolder;
        tempColors.resize(functionArcs.size());
        tempWidths = functionWidths;
        std::fill(tempColors.begin(), tempColors.end(), "grey");
        functionArcs.insert(functionArcs.end(),tempFunctionArcs.begin(),tempFunctionArcs.end());
        functionWidths.insert(functionWidths.end(),tempWidths.begin(),tempWidths.end());
        functionColors.insert(functionColors.end(), tempColors.begin(), tempColors.end());
    }
}

void H2Buffer::addKickedDrawing()
{
    addKickedDrawing(isometries);
}

void H2Buffer::setIsometries(const std::vector<H2Isometry> &isometries)
{
    this->isometries = isometries;
}
