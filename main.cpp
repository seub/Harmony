#include <QCoreApplication>
#include <QDebug>

#include "types.h"
#include "topologicalsurface.h"
#include "discretegroup.h"
#include "sl2cmatrix.h"
#include "cp1point.h"



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << std::endl;

    TopologicalSurface S(2,3);
    DiscreteGroup Gamma(S);
    //std::cout << Gamma << std::endl;

    SL2Cmatrix A(exp(.5),complex(3.0,10.0),0.0,exp(-.5));

    H3point p1,p2;
    p1.setBallCoordinates(0.0,0.0,0.0);
    p2.setBallCoordinates(.5,0.0,0.0);

    std::cout << H3distance(p1,p2) << std::endl;
    std::cout << H3distance(A*p1,A*p2) << std::endl;

    return a.exec();
}
