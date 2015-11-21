#include "mainapplication.h"
#include "h2polygontriangulater.h"
#include "tools.h"
#include "h2point.h"
#include "h2tangentvector.h"
#include "fenchelnielsenconstructor.h"

int main(int argc, char *argv[])
{

//    return MainApplication(argc, argv).exec();
    H2Point ptA,ptB;
    ptA.setDiskCoordinate(Complex(0.2,0.1));
    ptB.setDiskCoordinate(Complex(0.6,0.1));

    H2TangentVector vector(ptA,ptB);
    std::cout << vector << std::endl;
    std::cout << vector.exponentiate() << " is the same as " << ptB << "?" << std::endl;


}
