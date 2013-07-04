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

    TopologicalSurface S(2,3);
    DiscreteGroup Gamma(S);
    //std::cout << Gamma << std::endl;

    SL2Cmatrix A(2.0,complex(0.0,1.0),complex(0.0,-1.0),1.0);

    H3point p1,p2;
    p1.setBallCoordinates(0.0,0.0,0.0);
    p2.setBallCoordinates(.5,0.1,0.2);

    double x0,x1,x2,x3,x,y,z,p,q,r;
    SL2Cmatrix M;
    p1.getHalfSpaceCoordinates(x,y,z);
    p1.getHermitianCoordinates(M);
    p1.getHyperboloidCoordinates(x0,x1,x2,x3);
    p1.getKleinCoordinates(p,q,r);

    std::cout << "Hyperboloid coordinates: ( " << x0 << ", " << x1 << ", " << x2 << ", " << x3 << " )" << std::endl;
    std::cout << "Upper-half space coordinates: ( " << x << ", " << y << ", " << z << " )" << std::endl;
    std::cout << "Kleinian coordinates: ( " << p << ", " << q << ", " << r << " )" << std::endl;
    std::cout << "Hermitian coordinates: " << M << std::endl;


    return a.exec();
}
