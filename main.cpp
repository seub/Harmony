#include "mainapplication.h"
#include "h2polygontriangulater.h"
#include "tools.h"
#include "h2point.h"
#include "h2tangentvector.h"
#include "fenchelnielsenconstructor.h"

int main(int argc, char *argv[])
{

    //return MainApplication(argc, argv).exec();

    H2TangentVector U(H2Point::fromDiskCoordinate(Complex(0.5,0.3)), Complex(-1.0, -3.0));
    H2Point u, uBetter;
    std::cout << "U has length " << U.length() << std::endl;

    H2TangentVector par, parBetter;
    par = U.parallelTransport(3.8);
    parBetter = U.parallelTransportBetter(3.8);

    std::cout << "U.parallelTransport is " << par << " and has length = " << par.length() << std::endl;
    std::cout << "U.parallelTransportBetter is " << parBetter << " and has length = " << parBetter.length() << std::endl;



}
