#include "mainapplication.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;

/*    TopologicalSurface S(2,0);
    DiscreteGroup Gamma(S);
    IsomH2Representation rho(&Gamma);
    rho.setNiceRepresentation();

    std::vector<H2Isometry> aroundVertex, aroundVertices;
    aroundVertex = rho.getSidePairingsNormalizedAroundVertex();
    aroundVertices = rho.getSidePairingsNormalizedAroundVertices();

    std::cout << "The size of aroundVertex is " << aroundVertex.size() << std::endl;
    std::cout << "The size of aroundVertices is " << aroundVertices.size() << std::endl;
*/
    MainApplication mainApplication(argc, argv);
    return mainApplication.exec();
}
