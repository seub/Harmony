#include <QApplication>
#include <QDebug>
#include <QWidget>

#include "window.h"
#include "fenchelnielsenconstructor.h"
#include "h2mesh.h"
#include "h2meshfunction.h"
#include "h2canvasdelegate.h"
#include "canvas.h"
#include "equivariantharmonicmapsfactory.h"

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
        lengths1.push_back(2);
        lengths2.push_back(1.0);
    }

    EquivariantHarmonicMapsFactory F;
    F.setGenus(2);
    F.setRhoDomain(lengths1, twists1);
    //F.setNiceRhoDomain();
    //F.setRhoTarget(lengths1, twists1);
    F.setNiceRhoTarget();
    F.setMeshDepth(5);
    F.initialize();

    window.setFactory(&F);

    window.show();
    window.resizeCanvases();

    std::cout << std::endl;




    return a.exec();
}
