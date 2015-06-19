#include "mainapplication.h"
#include "fenchelnielsenconstructor.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;
/*
    TopologicalSurface S(2,0);
    DiscreteGroup Gamma(S);
    std::vector<double> lengths1, lengths2, twists1, twists2;
    for(int j=0; j<3; ++j)
    {
        lengths1.push_back(1.0+j);
        twists1.push_back(0.0);
        lengths2.push_back(1.0+j);
        twists2.push_back(0.0);
    }
    lengths2[1] = 10.0;
    FenchelNielsenConstructor FN1(lengths1,twists1), FN2(lengths2,twists2);
    IsomH2Representation rho1, rho2;
    rho1 = FN1.getUnnormalizedRepresentation(&Gamma);
    rho2 = FN2.getUnnormalizedRepresentation(&Gamma);

    std::vector<H2Isometry> generatorImages1, generatorImages2;
    generatorImages1 = rho1.getGeneratorImages();
    generatorImages2 = rho2.getGeneratorImages();
*/
/*    H2Isometry imageCurve1, imageCurve2, a11, a12, b11, b12, a21, a22, b21, b22;
    a11 = generatorImages1[0];
    a12 = generatorImages2[0];
    b11 = generatorImages1[1];
    b12 = generatorImages2[1];
    a21 = generatorImages1[2];
    a22 = generatorImages2[2];
    b21 = generatorImages1[3];
    b22 = generatorImages2[3];
    imageCurve1 = a11*b11*a11.inverse()*b11.inverse();
    imageCurve2 = a12*b12*a12.inverse()*b12.inverse();

    std::cout << "imageCurve1 has translation length " << imageCurve1.translationLength() << std::endl;
    std::cout << "imageCurve2 has translation length " << imageCurve2.translationLength() << std::endl;
*/
/*
    std::vector<double> lengthVec1, lengthVec2;
    for (const auto & A : generatorImages1)
    {
        lengthVec1.push_back(A.translationLength());
    }
    for (const auto & A : generatorImages2)
    {
        lengthVec2.push_back(A.translationLength());
    }

    std::cout << "Lengths for rho1 : " << lengthVec1  << std::endl;
    std::cout << "Lengths for rho2 : " << lengthVec2  << std::endl;

    std::cout << "rho1==rho2 ? " << (rho1==rho2) << std::endl;
*/
    MainApplication mainApplication(argc, argv);
    return mainApplication.exec();
}
