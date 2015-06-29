#include "mainapplication.h"
#include "h2triangle.h"
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
    lengths.push_back(0.1);
    lengths.push_back(4.0);
    lengths.push_back(5.0);
    twists.push_back(0.0);
    twists.push_back(-5.0);
    twists.push_back(3.0);
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

    std::cout << "Stupid basepoint = " << P2.getVertex(0) << std::endl;



    std::vector<H2Isometry> vertexPairings = rho.evaluateRepresentation(Gamma.getPairingsClosedSurfaceFromVertex());
    H2Point basePt;
    basePt.setDiskCoordinate(Complex(0.0666667,-0.3));
    std::vector<H2Point> images;
    images.reserve(8);
    for (const auto & A : vertexPairings)
    {
        images.push_back(A*basePt);
    }
    std::cout << "The diameter of an " << basePt << " = " << H2Point::diameter(images) << std::endl;
*/


    return MainApplication(argc, argv).exec();
}
