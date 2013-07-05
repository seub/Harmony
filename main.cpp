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
    std::cout << Gamma << std::endl;

    SL2Cmatrix A(2.0*exp(.5),2.5,.4,exp(-.5));

    H3point p1,p2;
    p1.setHalfSpaceCoordinates(23.0,0.0,2.45);
    p2.setHalfSpaceCoordinates(-1.1,0.0,1.4);

    double x,y,z;
    (((A*A*A*A).inverse())*p2).getHalfSpaceCoordinates(x,y,z);
    std::cout << "( " << x << ", " << y << ", " << z << " )" << std::endl;

    return a.exec();
}
