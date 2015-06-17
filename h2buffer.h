#ifndef H2BUFFER_H
#define H2BUFFER_H


#include <QColor>

#include "tools.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "h2polygon.h"
#include "grouprepresentation.h"
#include "h2mesh.h"
#include "h2trianglesubdivision.h"
#include "h2meshfunction.h"


class H2Buffer
{
    friend class H2CanvasDelegate;
    friend class H2CanvasDelegateDomain;
    friend class H2CanvasDelegateTarget;

public:
    H2Buffer();

    void addElement(const H2Point & point, const QColor & color = "black", int width = 3);
    void addElement(const std::vector<H2Point> & points, const QColor & color = "black", int width = 2);
    void addElement(const H2Geodesic & geodesic, const QColor & color = "black", int width = 1);
    void addElement(const H2GeodesicArc & geodesicArc, const QColor & color = "black", int width = 1);
    void addElement(const std::vector<H2GeodesicArc> & geodesicArcs, const QColor & color = "black", int width = 1);
    void addElement(const H2Polygon & polygon, const QColor & color = "black", int width = 3);
    void addElement(const H2Triangle & triangle, const QColor & color = "black", int width = 1);
    void addElement(const std::vector<H2Triangle> & triangles, const QColor & color = "black", int width = 1);
    void addElement(const H2Isometry & isometry, const QColor & color = "black", int width = 1);
    void addElement(const IsomH2Representation & rho, const QColor & color = "black", int width = 1);
    void addElement(const std::vector<H2Isometry> &V, const QColor & color = "black", int width = 1);
    void addElement(const std::vector<H2Polygon> &V, const QColor & color = "black", int width = 1);
    void addElement(const H2TriangleSubdivision &T, const QColor & color = "black", int width = 1);
    void addElement(const H2Mesh *mesh, const QColor & color = "black", int width = 1);
    void addElement(H2MeshFunction *function, const QColor & color = "black", int width = 1);
    void refreshFunction();
    void refreshMesh();
    void addSideTranslates(const std::vector<H2Isometry> &translationsAroundVertices, const QColor &color = "grey", int width = 1);
    void addSideTranslates(const QColor &color = "grey", int width = 1);

    void addMeshTranslatesAroundVertex(const QColor &color = "grey", int width = 1);
    void addMeshTranslatesAroundVertices(const QColor &color = "grey", int width = 1);
    void addMeshTranslates(bool aroundVertex, bool aroundVertices, const QColor &color = "grey", int width = 1);

    void setTranslations();
    void setTranslations(const IsomH2Representation &rho);


private:
    std::vector<H2Point> points;
    std::vector<QColor> pointsColors;
    std::vector<int> pointsWidths;

    std::vector<H2Geodesic> geodesics;
    std::vector<QColor> geodesicsColors;
    std::vector<int> geodesicsWidths;

    std::vector<H2GeodesicArc> geodesicArcs;
    std::vector<QColor> geodesicArcsColors;
    std::vector<int> geodesicArcsWidths;

    std::vector<H2Isometry> translationsAroundVertex, translationsAroundVertices;

    const H2Mesh *mesh;
    bool isMeshEmpty;
    std::vector<H2Point> meshPoints;
    std::vector<H2GeodesicArc> meshArcs;
    std::vector<H2GeodesicArc> meshSides;
    std::vector<H2Point> meshPointsTranslatesAroundVertex, meshPointsTranslatesAroundVertices;
    std::vector<H2GeodesicArc> meshArcsTranslatesAroundVertex, meshArcsTranslatesAroundVertices;
    std::vector<H2GeodesicArc> meshSidesTranslatesAroundVertex, meshSidesTranslatesAroundVertices;
    QColor meshColor, meshTranslatesColor;
    int meshWidth, meshTranslatesWidth;
    H2MeshFunction *function;
    bool isMeshFunctionEmpty;


    H2Triangle triangleHighlighted;
    std::vector<H2Point> pointsHighlightedRed;
    std::vector<H2Point> pointsHighlightedBlue;
    std::vector<H2Point> pointsHighlightedGreen;
};

#endif // H2BUFFER_H
