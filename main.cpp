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


    int g = 4;

    std::vector<double> lengths;
    std::vector<double> twists;
    for (int i=0; i<3*g-3; i++)
    {
        //twists.push_back(1.0-.3*i);
        twists.push_back(-1.0 + i*.1);
    }
    for (int i=0; i<3*g-3; i++)
    {
        lengths.push_back(1.0 + .3*i);
    }

    FenchelNielsenConstructor fn(lengths,twists);
    DiscreteGroup group;
    IsomH2Representation rho(&group);

    rho = fn.getRepresentation(&group);
    rho.checkRelations();


    Canvas canvas;
    H2CanvasDelegate delegate(&canvas);
    canvas.setDelegate(&delegate);



    {
    H2Point aT,bT,cT;
    aT.setDiskCoordinate(complex(0.0,0.8));
    bT.setDiskCoordinate(complex(-0.5, -0.5));
    cT.setDiskCoordinate(complex(0.5, -0.5));
    H2Triangle T(aT, bT, cT);
    //clock_t t0 = clock();
    H2Triangulation Ts(T, 1);
    //H2Triangulation Ts(&aT, &bT, &cT, 1);
    //clock_t t1 = clock();
    //std::cout << "time to construct triangulation: " << (t1-t0)*1.0/CLOCKS_PER_SEC << "s" << std::endl;


    delegate.buffer.addElement(Ts);
    }

    H2Isometry id;
    id.setIdentity();
    delegate.redrawBuffer(id);
    canvas.show();


    std::cout << std::endl;
    return a.exec();
}
