#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "tools.h"

#include "canvas.h"
#include "canvasdelegate.h"
#include "h2canvasdelegate.h"
#include "discretegroup.h"
#include "h3canvasdelegate.h"
#include "grouprepresentation.h"
#include "h2isometry.h"
#include "fenchelnielsenconstructor.h"
#include "drawelement.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    QApplication a(argc, argv);


    int g = 8;
    std::vector<double> lengths;
    std::vector<double> twists;
    for (int i=0; i<3*g-3; i++)
    {
        twists.push_back(.3-i*.05);
    }
    for (int i=0; i<3*g-3; i++)
    {
        lengths.push_back(2.0);
    }


    Canvas canvas;
    H2CanvasDelegate delegate(&canvas);
    DiscreteGroup group;

    FenchelNielsenConstructor fn(lengths,twists);
    IsomH2Representation rho2 = fn.getRepresentation(&group);
    rho2.checkRelations();

    /*IsomH2Representation rho(&group);
    rho.setFenchelNielsenCoordinates(lengths,twists);*/


    canvas.setDelegate(&delegate);
    //delegate.drawGeneratorAxes(rho);
    //DrawGeneratorAxes test(rho2, "red");
    //delegate.addElementToBuffer(test);
    H2Isometry id;
    id.setDiskCoordinates(I, 0);

    std::vector<H2Isometry> isometryList;
    isometryList = rho2.getSidePairingsForNormalizedFundamentalDomain();
    DrawListOfAxesOfIsometries isometriesList(isometryList);


    H2Point p;
    p.setDiskCoordinate(0.0);
    H2Polygon P = rho2.generateFundamentalDomainForNormalizedSurfaceGroup(p);
    DrawH2Polygon polygon(P);
    delegate.addElementToBuffer(isometriesList);
    delegate.addElementToBuffer(polygon);
    delegate.redrawBuffer(id);

    delegate.drawListOfAxesOfIsometries(isometryList,"red",1);
    delegate.drawH2Polygon(P);



/*    std::vector<H2Isometry> generatorImages = rho.getGeneratorImages();
    H2Isometry a1 = generatorImages[0];
    H2Isometry b1 = generatorImages[1];
    H2Isometry a2 = generatorImages[2];
    H2Isometry b2 = generatorImages[3];

    delegate.drawH2Polygon(a1*P, "blue");
    delegate.drawH2Polygon(b1*P, "green");
    delegate.drawH2Polygon(a2*P, "purple");
    delegate.drawH2Polygon(b2*P, "yellow");*/


    canvas.show();

    std::cout << std::endl;
    return a.exec();
}

