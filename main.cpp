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

    int g = 2;
    std::vector<double> lengths;
    std::vector<double> twists;
    twists.resize(3*g-3);
    twists[2] = 1.0;
    //twists[4] = 1.0;
    for (int i=0; i<3*g-3; i++)
    {
        lengths.push_back(2.5);
    }
    /*clock_t t0, t1;

    t0 = clock();

    for (int c=0; c<10000; c++)
    {
        lengths[0]+=0.0001;
        rho.setFenchelNielsenCoordinates(lengths, twists);
    }

    t1 = clock();
    double time = (t1-t0)*1.0/CLOCKS_PER_SEC;
    std::cout << "time spent: " << time << "s" << std::endl;*/
    //IsomH2Representation rho = IsomH2Representation::setFNCoordinatesUnnormalized(&Gamma, lengths, twists);

    DiscreteGroup Gamma;
    IsomH2Representation rho(&Gamma);
    rho.setFenchelNielsenCoordinates(lengths, twists);
    //std::cout << rho << std::endl;
    rho.checkRelations();


    //FenchelNielsenConstructor fn(lengths,twists);
    //DiscreteGroup group;
    //IsomH2Representation rho2 = fn.getrepresentation(&group);

    //std::cout << rho2 << std::endl;
    //std::cout << rho2 << std::endl;
    //rho.checkRelations();



    Canvas canvas;
    H2CanvasDelegate delegate(&canvas);
    canvas.setDelegate(&delegate);
    //delegate.drawGeneratorAxes(rho2);
    delegate.drawGeneratorAxes(rho, "red",1);
    H2Point p;
    p.setDiskCoordinate(0.0);
    H2Polygon P = rho.generatePolygon(p);
    delegate.drawH2Polygon(P,"black",5);

    std::vector<H2Isometry> generatorImages = rho.getGeneratorImages();
    H2Isometry a1 = generatorImages[0];
    H2Isometry b1 = generatorImages[1];
    H2Isometry a1inv = generatorImages[0].inverse();
    H2Isometry b1inv = generatorImages[1].inverse();
    H2Isometry a2 = generatorImages[2];
    H2Isometry b2 = generatorImages[3];
    H2Isometry a2inv = generatorImages[2].inverse();
    H2Isometry b2inv = generatorImages[3].inverse();

    delegate.drawH2Polygon(a1inv*P, "blue");
    delegate.drawH2Polygon(b1inv*a1inv*P, "blue");
    delegate.drawH2Polygon(a1*b1inv*a1inv*P, "blue");
    delegate.drawH2Polygon(b1*a1*b1inv*a1inv*P, "blue");
    delegate.drawH2Polygon(a2inv*b1*a1*b1inv*a1inv*P, "blue");
    delegate.drawH2Polygon(b2inv*a2inv*b1*a1*b1inv*a1inv*P, "blue");
    delegate.drawH2Polygon(a2*b2inv*a2inv*b1*a1*b1inv*a1inv*P, "blue");
    delegate.drawH2Polygon(b2*a2*b2inv*a2inv*b1*a1*b1inv*a1inv*P, "blue");

    std::cout << b2*a2*b2inv*a2inv*b1*a1*b1inv*a1inv << std::endl;

    canvas.show();

    std::cout << std::endl;
    return a.exec();
}

