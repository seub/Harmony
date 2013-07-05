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

    TopologicalSurface S(2,3);
    DiscreteGroup Gamma(S);
    //std::cout << Gamma << std::endl;


    Canvas *canvastest = new Canvas;
    canvastest->show();
    return a.exec();
}
