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
#include "h2mesh.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    QApplication a(argc, argv);

    //H3canvasDelegate windows(60, 0,"Test");
    //windows.show();

    int g = 2;

    std::vector<double> lengths;
    std::vector<double> twists;
    for (int i=0; i<3*g-3; i++)
    {
        //twists.push_back(-1.0 + i*.1);
        twists.push_back(0.0);
        twists.push_back(-1.0 + i*.1);
    }
    for (int i=0; i<3*g-3; i++)
    {
        //lengths.push_back(1.0 + .3*i);
        lengths.push_back(2.0);
    }

    FenchelNielsenConstructor fn(lengths,twists);
    DiscreteGroup group;
    IsomH2Representation rho(&group);

    rho = fn.getRepresentation(&group);
    rho.checkRelations();


    Canvas canvas;
    H2CanvasDelegate delegate(&canvas);
    canvas.setDelegate(&delegate);

    H2Triangle T;
    H2Point p1, p2, p3;
    p1.setDiskCoordinate(complex(0.0, 0.9));
    p2.setDiskCoordinate(complex(-0.7, -0.7));
    p3.setDiskCoordinate(complex(0.7, -0.7));
    T.setPoints(p1, p2, p3);

    {//H2Triangulation Ts(&p1, &p2, &p3, 6);
    H2Triangulation Ts(T, 6);

    delegate.buffer.addElement(Ts);}


    H2Isometry id;
    id.setIdentity();
    delegate.redrawBuffer(id);
    canvas.show();


    std::cout << std::endl;
    return a.exec();
}
