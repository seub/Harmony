#include "drawelement.h"
#include "canvas.h"
#include "h2canvasdelegate.h"


DrawH2Point::DrawH2Point(const H2Point & p, const  QColor  &color, int width, int identifiant):
    DrawElement(color, width, identifiant), p(p)
{

}

void DrawH2Point::drawElement(H2CanvasDelegate *canvas)
{
    canvas->drawH2Point(p, color, width);
}

DrawH2Geodesic::DrawH2Geodesic(const H2Geodesic & L, const QColor & color, int width, int identifiant):
    DrawElement(color, width, identifiant), L(L)
{

}

void DrawH2Geodesic::drawElement(H2CanvasDelegate *canvas)
{
    canvas->drawH2Geodesic(L, color, width);
}

DrawH2GeodesicArc::DrawH2GeodesicArc(const H2GeodesicArc & L, const QColor & color, int width, int identifiant):
    DrawElement(color, width, identifiant), L(L)
{

}

void DrawH2GeodesicArc::drawElement(H2CanvasDelegate *canvas)
{
    canvas->drawH2GeodesicArc(L, color, width);
}

DrawH2Polygon::DrawH2Polygon(const H2Polygon & P, const QColor & color, int width, int identifiant):
    DrawElement(color, width, identifiant), P(P)
{

}

void DrawH2Polygon::drawElement(H2CanvasDelegate *canvas)
{
    canvas->drawH2Polygon(P, color, width);
}

DrawGeneratorAxes::DrawGeneratorAxes(const IsomH2Representation & rho, const QColor & color, int width, int identifiant):
    DrawElement(color, width, identifiant), rho(rho)
{

}

void DrawGeneratorAxes::drawElement(H2CanvasDelegate *canvas)
{
    canvas->drawGeneratorAxes(rho, color, width);
}

DrawListOfAxesOfIsometries::DrawListOfAxesOfIsometries(const std::vector<H2Isometry> &isometries,
                                                       const QColor &color, int width, int identifiant) :
    DrawElement(color, width, identifiant), isometries(isometries)
{

}

void DrawListOfAxesOfIsometries::drawElement(H2CanvasDelegate *canvas)
{
    canvas->drawListOfAxesOfIsometries(isometries,color,width);
}
