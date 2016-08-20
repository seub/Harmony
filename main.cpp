#include "mainapplication.h"
#include "h2polygontriangulater.h"
#include "tools.h"
#include "h2point.h"
#include "h2tangentvector.h"
#include "fenchelnielsenconstructor.h"

int main(int argc, char *argv[])
{

    return MainApplication(argc, argv).exec();

/*
    H2TangentVector U(H2Point::fromDiskCoordinate(Complex(0.5,0.3)), Complex(-.01, -.3));
    H2TangentVector V(H2Point::fromDiskCoordinate(Complex(0.5,0.3)), Complex(1.0,2.0));

    H2Point u,v;
    u = U.exponentiate();
    v = V.exponentiate(0.0);

    std::cout << "u = " << u << std::endl;
    std::cout << "v = " << v << std::endl;
*/



}
