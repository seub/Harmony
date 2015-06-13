#include <QApplication>
#include <QDebug>
#include <QWidget>

#include "window.h"
#include "fenchelnielsenconstructor.h"
#include "h2mesh.h"
#include "h2meshfunction.h"
#include "h2canvasdelegate.h"
#include "canvas.h"

int main(int argc, char *argv[])
{

    std::cout << std::endl;
    QApplication a(argc, argv);

    Window window;


    int g = 2;

    std::vector<double> lengths1,lengths2;
    std::vector<double> twists1,twists2;
    for (int i=0; i<3*g-3; i++)
    {
        //twists.push_back(1.0-.3*i);
        twists1.push_back(0.0);
        twists2.push_back(0.0);
    }
    twists2[1] = 2*M_PI;

    for (int i=0; i<3*g-3; i++)
    {
        lengths1.push_back(1.0);
        lengths2.push_back(1.0);
    }

    FenchelNielsenConstructor fn1(lengths1,twists1);
    FenchelNielsenConstructor fn2(lengths2,twists2);
    DiscreteGroup group;
    IsomH2Representation rhoImage(&group), rhoDomain(&group);

    rhoDomain = fn1.getRepresentation(&group);
    rhoImage = fn2.getRepresentation(&group);

    //rhoImage.setNiceRepresentation();
    //rho.setNiceRepresentation();


    H2Mesh mesh;
    int depth = 4;
    mesh = H2Mesh(rhoDomain, depth);




    Canvas* canvas1 = window.leftCanvas;
    ((H2CanvasDelegateDomain *) canvas1->delegate)->buffer.addElement(rhoDomain, "blue", 2);
    ((H2CanvasDelegateDomain *) canvas1->delegate)->buffer.addElement(&mesh);
    ((H2CanvasDelegateDomain *) canvas1->delegate)->redrawBuffer();


    H2MeshFunction f(&mesh, rhoImage);
    f.initializePLsmart();

    Canvas* canvas2 = window.rightCanvas;
    ((H2CanvasDelegateDomain *) canvas1->delegate)->buffer.addElement(rhoImage, "blue", 2);
    ((H2CanvasDelegateTarget *) canvas2->delegate)->buffer.addElement(&f, "red", 1);
    ((H2CanvasDelegateTarget *) canvas2->delegate)->redrawBuffer();

    window.show();
    window.resizeCanvases();

    std::cout << std::endl;




    return a.exec();
}
