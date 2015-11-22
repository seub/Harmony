#include "mainapplication.h"
#include "h2polygontriangulater.h"
#include "tools.h"
#include "h2point.h"
#include "h2tangentvector.h"
#include "fenchelnielsenconstructor.h"

int main(int argc, char *argv[])
{

    //return MainApplication(argc, argv).exec();

    H2TangentVector U(H2Point::fromDiskCoordinate(Complex(0.5,0.3)), Complex(.5, -.07));

    std::cout << U.length() << std::endl;
    std::cout << 2*(U.parallelTransport(-3.0)).length() << std::endl;
}
