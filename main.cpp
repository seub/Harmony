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

/*    H3canvasDelegate windows(60, 0,"Test");
    windows.show();*/

    int g=2;
    std::vector<double> lengths,twists;
    twists.resize(3*g-3);
    for(unsigned int k=0;k<3*g-3;k++)
    {
        lengths.push_back(1.0);
    }

    DiscreteGroup Gamma;
    //IsomH2Representation rho(&Gamma);

    IsomH2Representation rho = IsomH2Representation::setFNCoordinatesUnnormalized(&Gamma, lengths, twists);
    std::cout << rho << std::endl;
    rho.checkRelations();



    H2Point zero;
    zero.setDiskCoordinate(0.0);
    H2Polygon P = rho.generatePolygon(zero);

    Canvas * canvas = new Canvas;
    H2CanvasDelegate delegate(canvas);
    canvas->setDelegate(&delegate);
    delegate.drawGeneratorAxes(rho);
    //delegate.drawH2Polygon(P);
    canvas->show();


    std::cout << std::endl;
    return a.exec();
}





