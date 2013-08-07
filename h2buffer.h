#ifndef H2BUFFER_H
#define H2BUFFER_H

#include "tools.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "h2polygon.h"
#include "grouprepresentation.h"
#include "h2mesh.h"
#include "h2triangulation.h"
#include <QColor>


class H2Buffer
{
    friend class H2CanvasDelegate;

public:
    H2Buffer();

    void reset();

    void addElement(const H2Point & point, const QColor & color = "black", int width = 3);
    void addElement(const H2Geodesic & geodesic, const QColor & color = "black", int width = 1);
    void addElement(const H2GeodesicArc & geodesicArc, const QColor & color = "black", int width = 1);
    void addElement(const H2Polygon & polygon, const QColor & color = "black", int width = 1);
    void addElement(const H2Triangle & triangle, const QColor & color = "black", int width = 1);
    void addElement(const std::vector<H2Triangle> & triangles, const QColor & color = "black", int width = 1);
    void addElement(const H2Isometry & isometry, const QColor & color = "black", int width = 1);
    void addElement(const IsomH2Representation & rho, const QColor & color = "black", int width = 1);
    void addElement(const std::vector<H2Isometry> &V, const QColor & color = "black", int width = 1);
    void addElement(const std::vector<H2Polygon> &V, const QColor & color = "black", int width = 1);
    void addElement(const H2mesh & mesh, const QColor & color = "black", int width = 1);
    void addElement(const H2Triangulation *triangulation, const QColor & color = "black", int width = 1);


//private:
    std::vector<H2Point> points;
    std::vector<QColor> pointsColors;
    std::vector<int> pointsWidths;

    std::vector<H2Geodesic> geodesics;
    std::vector<QColor> geodesicsColors;
    std::vector<int> geodesicsWidths;

    std::vector<H2GeodesicArc> geodesicArcs;
    std::vector<QColor> geodesicArcsColors;
    std::vector<int> geodesicArcsWidths;
};

#endif // H2BUFFER_H
