#include "mainapplication.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;

/*
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
*/

    return MainApplication(argc, argv).exec();
}
