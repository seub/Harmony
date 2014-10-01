#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "tools.h"
#include "h2mesh.h"
#include "fenchelnielsenconstructor.h"
#include "h2canvasdelegate.h"
#include "canvas.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    QApplication a(argc, argv);


    int g = 2;

    std::vector<double> lengths;
    std::vector<double> twists;
    for (int i=0; i<3*g-3; i++)
    {
        //twists.push_back(1.0-.3*i);
        twists.push_back(0.2);
    }
    for (int i=0; i<3*g-3; i++)
    {
        lengths.push_back(1.0 + .3*i);
    }

    FenchelNielsenConstructor fn(lengths,twists);
    DiscreteGroup group;
    IsomH2Representation rho(&group);

    rho = fn.getRepresentation(&group);

    rho.setNiceRepresentation();
    rho.checkRelations();

    H2Mesh mesh;
    int k = 4;
    //for (k=0; k<10; ++k)
    //{
    std::cout << "depth = " << k << std::endl;
    clock_t start = clock();
    mesh = H2Mesh(rho, k);
    clock_t end = clock();
    std::cout << "Time to construct mesh: " << (end-start)*1.0/CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << std::endl;
    //}


    Canvas canvas;
    H2CanvasDelegate delegate(&canvas);
    canvas.setDelegate(&delegate);
    delegate.buffer.addElement(rho, "blue", 2);
    delegate.buffer.addElement(mesh);


    delegate.redrawBuffer();
    canvas.show();

    std::cout << "random bob: " << rand() % 100 << std::endl;




    std::cout << std::endl;
    return a.exec();
}
