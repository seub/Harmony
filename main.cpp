#include "mainapplication.h"
#include "fenchelnielsenconstructor.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;
/*
    TopologicalSurface S(2,0);
    DiscreteGroup Gamma(S);
    GroupRepresentation<H2Isometry> rho(Gamma);
    std::vector<double> lengths, twists;
    lengths.push_back(3.0);
    lengths.push_back(2.0);
    lengths.push_back(2.0);
    twists.push_back(0.0);
    twists.push_back(0.0);
    twists.push_back(0.0);
    FenchelNielsenConstructor F(lengths,twists);
    rho = F.getRepresentation();
    H2Polygon P = rho.generateFundamentalDomain(50);
    H2PolygonTriangulater T(&P);
*/

    return MainApplication(argc, argv).exec();
}
