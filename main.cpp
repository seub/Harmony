#include "mainapplication.h"
#include "h2polygontriangulater.h"
#include "tools.h"
#include "h2point.h"
#include "h2tangentvector.h"
#include "fenchelnielsenconstructor.h"

int main(int argc, char *argv[])
{

    //return MainApplication(argc, argv).exec();


    H2TangentVector U(H2Point::fromDiskCoordinate(Complex(0.5,0.3)), Complex(-.01, -.3));
    H2TangentVector V(H2Point::fromDiskCoordinate(Complex(0.5,0.3)), Complex(1.0,2.0));

    H2TangentVector Uout1, Uout2, Vout1, Vout2;
    Uout1 = U.parallelTransport(-1.0);
    Uout2 = U.parallelTransportOld(-1.0);
    Vout1 = V.parallelTransport(-1.0);
    Vout2 = V.parallelTransportOld(-1.0);

    std::cout << "U = " << U << std::endl;

    std::cout << "Uout1 = " << Uout1 << std::endl;
    std::cout << "Uout2 = " << Uout2 << std::endl;

    std::cout << "V = " << V << std::endl;

    std::cout << "Vout1 = " << Vout1 << std::endl;
    std::cout << "Vout2 = " << Vout2 << std::endl;




}
