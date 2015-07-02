#include "mainapplication.h"
#include "h2polygontriangulater.h"
#include "tools.h"
#include "h2point.h"
#include "fenchelnielsenconstructor.h"

#include "tests.cpp"

int main(int argc, char *argv[])
{
/*
    std::cout << std::endl;


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


    H2Polygon P1 = rho.generateFundamentalDomainOptimization();
    //H2Polygon P2 = rho.generateFundamentalDomain(30);

    H2PolygonTriangulater T(&P1);

*/

/*
    QApplication a(argc, argv);
    runTests();
    return a.exec();
*/
    return MainApplication(argc, argv).exec();
}
