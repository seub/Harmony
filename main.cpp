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


    int g = 3;

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
    rho.checkRelations();

    clock_t start = clock();
    H2Mesh mesh(rho, 2);
    clock_t end = clock();
    std::cout << "Time to construct mesh: " << (end-start)*1.0/CLOCKS_PER_SEC << "s" << std::endl;


    Canvas canvas;
    H2CanvasDelegate delegate(&canvas);
    canvas.setDelegate(&delegate);
    delegate.buffer.addElement(rho, "blue", 2);
    delegate.buffer.addElement(mesh);


    delegate.redrawBuffer();
    canvas.show();



    std::cout << std::endl;
    return a.exec();
}
