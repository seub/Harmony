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
    std::cout << Gamma << std::endl;


    return a.exec();
}
