#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "types.h"
#include "topologicalsurface.h"
#include "discretegroup.h"
#include "sl2cmatrix.h"
#include "cp1point.h"
#include "canvas.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::cout << std::endl;

    TopologicalSurface S(0,4);
    DiscreteGroup Gamma(S);
    std::cout << Gamma << std::endl;

    std::vector<SL2Cmatrix> list(4);
    list[1] = SL2Cmatrix(1.0,0.0,0.0,1.0);

//    Canvas *canvastest = new Canvas;
//    canvastest->show();

    return a.exec();
}
