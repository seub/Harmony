#include "mainapplication.h"
#include "h2polygontriangulater.h"
#include "tools.h"
#include "h2point.h"
#include "fenchelnielsenconstructor.h"


int main(int argc, char *argv[])
{

    std::cout << std::endl;

/*
    TopologicalSurface S(2,0);
    DiscreteGroup Gamma(S);
    GroupRepresentation<H2Isometry> rho(Gamma);
    std::vector<double> lengths, twists;
    lengths.push_back(2.0);
    lengths.push_back(2.0);
    lengths.push_back(2.0);
    twists.push_back(0.0);
    twists.push_back(0.0);
    twists.push_back(0.0);
    FenchelNielsenConstructor F(lengths,twists);
    rho = F.getRepresentation();


    clock_t start,end;
    start = clock();
    H2Polygon P1 = rho.generateFundamentalDomainOptimization();
    end = clock();
    std::cout << (end - start)*1.0/CLOCKS_PER_SEC << " seconds for generateFundmentalDomainOptimization" << std::endl;
    start = clock();
    H2Polygon P2 = rho.generateFundamentalDomain(30);
    end = clock();
    std::cout << (end - start)*1.0/CLOCKS_PER_SEC << " seconds for generateFundmentalDomain" << std::endl;

    H2PolygonTriangulater T(&P1);
*/


    return MainApplication(argc, argv).exec();
}
