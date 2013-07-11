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
    for (int i=0; i<3*g-3; i++)
    {
        lengths.push_back(1.2+i);
    }
    DiscreteGroup Gamma;
    IsomH2Representation rho(&Gamma);
    clock_t t0, t1;

    t0 = clock();

    for (int c=0; c<10000; c++)
    {
        lengths[0]+=0.0001;
        rho.setFenchelNielsenCoordinates(lengths, twists);
    }

    t1 = clock();
    double time = (t1-t0)*1.0/CLOCKS_PER_SEC;
    std::cout << "time spent: " << time << "s" << std::endl;
    //IsomH2Representation rho = IsomH2Representation::setFNCoordinatesUnnormalized(&Gamma, lengths, twists);

    //std::cout << rho << std::endl;
    //rho.checkRelations();

    Canvas canvas;
    H2CanvasDelegate delegate(&canvas);
    canvas.setDelegate(&delegate);
    delegate.drawGeneratorAxes(rho);
    //H2Point p;
    //p.setDiskCoordinate(0.0);
    //delegate.drawH2Polygon(rho.generatePolygon(p));
    //canvas.show();

    std::cout << std::endl;
    return a.exec();
}

