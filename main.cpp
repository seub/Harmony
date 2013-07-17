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
#include "circle.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    QApplication a(argc, argv);

    //H3canvasDelegate windows(60, 0,"Test");
    //windows.show();


    int g = 4;

    std::vector<double> lengths;
    std::vector<double> twists;
    for (int i=0; i<3*g-3; i++)
    {
        //twists.push_back(1.0-.3*i);
        twists.push_back(-1.0);
    }
    for (int i=0; i<3*g-3; i++)
    {
        lengths.push_back(2.5);
    }

    FenchelNielsenConstructor fn(lengths,twists);
    DiscreteGroup group;
    IsomH2Representation rho = fn.getRepresentation(&group);
    rho.checkRelations();


    Canvas canvas;
    H2CanvasDelegate delegate(&canvas);
    canvas.setDelegate(&delegate);

    H2Point p;
    p.setDiskCoordinate(0.0);
    H2Polygon P = rho.generatePolygon(100);
    delegate.buffer.addElement(rho, "red");
    delegate.buffer.addElement(P, "blue", 4);

    /*std::vector<H2Isometry> listOfIsoms = rho.getSidePairingsNormalizedAroundVerticesToDepth(1);
    std::vector<H2Polygon> listOfPolys = listOfIsoms*P;

    delegate.buffer.addElement(listOfPolys);*/


    H2Isometry id;
    id.setIdentity();
    delegate.redrawBuffer(id);
    canvas.show();


    std::cout << std::endl;
    return a.exec();
}
