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
    H2Point pt;
    pt.setDiskCoordinate(Complex(0.0,.869022));
    H2TangentVector u(pt,Complex(0.0000186,-.0000327934));
    std::cout << "pt has norm " << norm(pt.getDiskCoordinate()) << std::endl;
    std::cout << "The vector u is " << u << " and the exponential is " << u.exponentiate() << std::endl;
    std::cout << "u has (hyperbolic) length " << u.length() << std::endl;
    std::cout << "The distance between root and exponential image is " << H2Point::distance(pt,u.exponentiate()) << std::endl;
    u= H2TangentVector(pt,Complex(0.00001867,-.0000327934));
    std::cout << "The vector u is " << u << " and the exponential is " << u.exponentiate() << std::endl;
    std::cout << "u has (hyperbolic) length " << u.length() << std::endl;
    std::cout << "The distance between root and exponential image is " << H2Point::distance(pt,u.exponentiate()) << std::endl;
*/

}
