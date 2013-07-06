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
    std::cout << std::endl;
    QApplication a(argc, argv);

    TopologicalSurface S(2,3);
    DiscreteGroup Gamma(S);
    //std::cout << Gamma << std::endl;


    Canvas *canvastest = new Canvas;
    canvastest->show();

    std::cout << std::endl;
    return a.exec();
}
