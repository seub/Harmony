#ifndef H2CANVASDELEGATE_H
#define H2CANVASDELEGATE_H

#include "tools.h"
#include "canvasdelegate.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "h2polygon.h"
#include "grouprepresentation.h"

enum drawElementType {POINT, GEODESIC, GEODESICARC, POLYGON, GENERATORAXES};

class DrawElement;

class H2CanvasDelegate : public CanvasDelegate
{
public:
    H2CanvasDelegate(Canvas *canvas);

    void drawH2Point(const H2Point & p, const QColor & color = "black", int width = 4);
    void drawH2Geodesic(const H2Geodesic & L, const QColor & color = "black", int width = 1);
    void drawH2GeodesicArc(const H2GeodesicArc &L, const QColor & color = "black", int width = 2);
    void drawH2Polygon(const H2Polygon & P, const QColor & color = "black", int width = 2);
    void drawListOfAxesOfIsometries(const std::vector<H2Isometry> & isometries, const QColor & color = "black", int width = 2);

    void drawExample();
    void drawGeneratorAxes(const IsomH2Representation & rho, const QColor & color = "black", int width = 2);
    void redrawBuffer(const H2Isometry &mobius = H2Isometry::identity());
    void addElementToBuffer(DrawElement &element);
    void setMouse(const int mouseX, const int mouseY);
    void moveMouse(const int mouseX, const int mouseY);
    void stopMovingMouse(const int mouseX, const int mouseY);
private:
    std::vector<DrawElement *> buffer;
    H2Isometry mobius;
    H2Isometry savedmobius;
};

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


#endif // H2CANVASDELEGATE_H
