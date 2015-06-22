#include "mainapplication.h"
#include "h2buffer.h"
#include "fenchelnielsenconstructor.h"

int main(int argc, char *argv[])
{
<<<<<<< HEAD
    std::cout << std::endl;

    TopologicalSurface S(2,0);
    DiscreteGroup Gamma(S);
    IsomH2Representation rho(&Gamma);
    rho.setNiceRepresentation();
    H2Polygon P = rho.generatePolygon(50);


    H2PolygonTriangulater T(&P);
    T.createSteinerPointsDetailed();

    H2Polygon Q = T.fullPolygon;
    H2SteinerPolygon StP = T.steinerPolygon;

    std::cout << "P has " << P.getSides().size() << " sides" << std::endl;
    std::cout << "Q has " << Q.getSides().size() << " sides" << std::endl;
    std::cout << "StP's Steiner vertices = " << StP.getVectorNbSteinerPoints() << std::endl;


    MainApplication mainApplication(argc, argv);
    return mainApplication.exec();
=======
    return MainApplication(argc, argv).exec();
>>>>>>> 1b7e4fbdb4159858088c847450fdf6fa71408048
}
