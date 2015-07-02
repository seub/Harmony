#include "mainapplication.h"

#include "fenchelnielsenconstructor.h"

//#include "tests.cpp"

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //runTests();
    //return a.exec();


    /*std::vector<double> lengths = {1, 0.3, 3, 0.8, 2, 2.5};
    std::vector<double> twists = {-1, 1, 0, 0, 1, -0.5};

    //std::vector<double> lengths = {2, 2, 2};
    //std::vector<double> twists = {0, 0, 0};

    GroupRepresentation<H2Isometry> rho = FenchelNielsenConstructor(lengths, twists).getRepresentation();

    int N = 50;
    double x, y, f;
    H2Point p;
    H2Polygon P;
    std::vector<double> pointsX, pointsY, pointsF;
    for (x=-1; !(x>1); x+=2.0/N)
    {
        for(y=-1; !(y>1); y+=2.0/N)
        {
            if (x*x + y*y < 1.0)
            {
                p.setDiskCoordinate(Complex(x,y));
                P = rho.generateFundamentalDomain(p);

                if (P.isConvex())
                {
                    f = P.diameter();
                }
                else
                {
                    //f = -10.0;
                    f = P.diameter();
                }
                if (f>-20 && f<1000)
                {
                    pointsX.push_back(x);
                    pointsY.push_back(y);
                    pointsF.push_back(f);
                }
            }
        }
    }

    std::cout << std::endl;
    std::cout << "[";
    uint k;
    for (k=0; k+1!=pointsX.size(); ++k)
    {
        std::cout << "(" << pointsX[k] << ", " << pointsY[k] << ", " << pointsF[k] << "), ";
    }
    std::cout << "("  << pointsX[k] << ", " << pointsY[k] << ", " << pointsF[k] << ")]" << std::endl << std::endl;


    std::cout << "min F = " << *std::min_element(pointsF.begin(), pointsF.end()) << std::endl;
    std::cout << "max F = " << *std::max_element(pointsF.begin(), pointsF.end()) << std::endl;
    return 0;*/
    return MainApplication(argc, argv).exec();
}
