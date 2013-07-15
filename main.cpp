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

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    QApplication a(argc, argv);

    //H3canvasDelegate windows(60, 0,"Test");
    //windows.show();


    int g = 10;

    std::vector<double> lengths;
    std::vector<double> twists;
    twists.resize(3*g-3);
    //twists[2] = 1.0;
    //twists[4] = 1.0;
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
    H2Polygon P = rho.generateFundamentalDomainForNormalizedSurfaceGroup(p);
    delegate.buffer.addElement(rho, "red");
    delegate.buffer.addElement(P, "blue", 3);

    /*std::vector<H2Isometry> listOfIsoms1 = rho.getSidePairingsNormalizedToDepth(2);
    std::vector<H2Isometry> listOfIsoms2 = rho.getSidePairingsNormalizedAroundVertex();
    std::vector<H2Isometry> listOfIsoms;
    listOfIsoms.reserve( listOfIsoms1.size() + listOfIsoms2.size() );
    listOfIsoms.insert( listOfIsoms.end(), listOfIsoms1.begin(), listOfIsoms1.end() );
    listOfIsoms.insert( listOfIsoms.end(), listOfIsoms2.begin(), listOfIsoms2.end() );

    std::vector<H2Polygon> listOfPolys = listOfIsoms*P;
    delegate.buffer.addElement(listOfPolys);*/

    H2Isometry id;
    id.setIdentity();
    delegate.redrawBuffer(id);


    canvas.show();



    std::cout << std::endl;
    return a.exec();
}

