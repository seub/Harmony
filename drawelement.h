#ifndef DRAWELEMENT_H
#define DRAWELEMENT_H

#include "tools.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "h2polygon.h"
#include "grouprepresentation.h"

#include <QColor>

class H2CanvasDelegate;

class DrawElement
{
public:
    DrawElement(const QColor &color, int width, int identifiant):
         color(color), width(width), identifiant(identifiant){}
    virtual drawElementType getElementType() = 0;
    virtual void drawElement(H2CanvasDelegate *canvas) = 0;
protected:
    QColor color;
    int width;
    int identifiant;
};

class DrawH2Point : public DrawElement
{
public:
    DrawH2Point(const H2Point & p, const QColor &color = "black", int width = 4, int identifiant = 0);
    drawElementType getElementType() {return POINT;}
    void drawElement(H2CanvasDelegate *canvas);
private:
    H2Point p;
};

class DrawH2Geodesic : public DrawElement
{
public:
    DrawH2Geodesic(const H2Geodesic & L, const QColor & color = "black", int width = 4, int identifiant = 0);
    drawElementType getElementType() {return GEODESIC;}
    void drawElement(H2CanvasDelegate *canvas);
private:
    H2Geodesic L;
};

class DrawH2GeodesicArc : public DrawElement
{
public:
    DrawH2GeodesicArc(const H2GeodesicArc & L, const QColor & color = "black", int width = 4, int identifiant = 0);
    drawElementType getElementType() {return GEODESICARC;}
    void drawElement(H2CanvasDelegate *canvas);
    H2GeodesicArc L;
};

class DrawH2Polygon : public DrawElement
{
public:
    DrawH2Polygon(const H2Polygon & P, const QColor & color = "black", int width = 4, int identifiant = 0);
    drawElementType getElementType() {return POLYGON;}
    void drawElement(H2CanvasDelegate *canvas);
private:
    H2Polygon P;
};

class DrawGeneratorAxes : public DrawElement
{
public:
    DrawGeneratorAxes(const IsomH2Representation & rho, const QColor & color = "black", int width = 4, int identifiant = 0);
    drawElementType getElementType() {return GENERATORAXES;}
    void drawElement(H2CanvasDelegate *canvas);
private:
    IsomH2Representation rho;
};

class DrawListOfAxesOfIsometries : public DrawElement
{
public:
    DrawListOfAxesOfIsometries(const std::vector<H2Isometry> & isometries, const QColor & color = "black", int width = 2, int identifiant = 0);
    drawElementType getElementType() {return LISTOFAXES;}
    void drawElement(H2CanvasDelegate *canvas);
private:
    std::vector<H2Isometry> isometries;
};

#endif // DRAWELEMENT_H
