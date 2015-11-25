#include "mainapplication.h"
#include "h2polygontriangulater.h"
#include "tools.h"
#include "h2point.h"
#include "h2tangentvector.h"
#include "fenchelnielsenconstructor.h"

int main(int argc, char *argv[])
{

    //return MainApplication(argc, argv).exec();

    H2TangentVector U(H2Point::fromDiskCoordinate(Complex(0.5,0.3)), 10.0*Complex(-1.0, -3.0));
    H2Point u, uBetter;
    std::cout << "U has length " << U.length() << std::endl;

    u = U.exponentiate();
    uBetter = U.exponentiateBetter();

    H2TangentVector par, parBetter;
    par = U.parallelTransport(1.0);
    parBetter = U.parallelTransportBetter(1.0);

    std::cout << "U.parallelTransport(1.0) is " << par << " and has length = " << par.length() << std::endl;
    std::cout << "U.parallelTransportBetter(1.0) is " << parBetter << " and has length = " << parBetter.length() << std::endl;



}
