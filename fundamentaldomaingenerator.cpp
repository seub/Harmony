#include "fundamentaldomaingenerator.h"

#include <QTime>

FundamentalDomainGenerator::FundamentalDomainGenerator(const GroupRepresentation<H2Isometry> &rho) : rho(rho)
{
    assert(rho.getDiscreteGroup().isClosedSurfaceGroup());

    tol = 0.00001;
    xInit.setDiskCoordinate(Complex(0.0, 0.0));

    delta = 0.0000001;
    delX = Complex(1.0, 0.0);
    delY = Complex(0.0, 1.0);

    maxIterations = 100;
    maxLineSearchIterations = 50;
}

H2Polygon FundamentalDomainGenerator::getOptimalFundamentalDomain() const
{
    H2Point firstVertex = optimalStepGradientDescent(xInit);
    H2Polygon out = rho.getFundamentalDomain(firstVertex);

    if (!out.isConvex())
    {
        std::cout << "Warning in FundamentalDomainGenerator::getOptimalFundamentalDomain(): the polygon found is not convex" << std::endl;
    }
    return rho.getFundamentalDomain(firstVertex);
}


double FundamentalDomainGenerator::F(const H2Point &x) const
{
    return diameterNorm(rho.getFundamentalDomain(x));
    //return squareDiameterNorm(rho.getFundamentalDomain(x));
    //return minAngleNorm(rho.getFundamentalDomain(x));
    //return maxAngleNorm(rho.getFundamentalDomain(x));
    //return minDistanceToNonNeighborSideNorm(rho.getFundamentalDomain(x));
    //return energyNorm(rho.getFundamentalDomain(x));
    //return distanceToIdealBoundaryNorm(rho.getFundamentalDomain(x));
    //return isoperimetricNorm(rho.getFundamentalDomain(x));
}

double FundamentalDomainGenerator::Phi(const H2Point &x0, const Complex &u, const double &t) const
{
    return F(H2Point::exponentialMap(x0, u, t));
}

Complex FundamentalDomainGenerator::gradF(const H2Point &x) const
{
    double delXF = dPhi(x, delX, 0);
    double delYF = dPhi(x, delY, 0);
    Complex z = x.getDiskCoordinate();
    double g = (1 - norm(z))*(1-norm(z))/4;
    return g*Complex(delXF, delYF);
}

double FundamentalDomainGenerator::dPhi(const H2Point &x0, const Complex &u, const double &t) const
{
    return (Phi(x0, u, t+delta)-Phi(x0, u, t-delta))/(2*delta);
}

double FundamentalDomainGenerator::diameterNorm(const H2Polygon &polygon)
{
    return polygon.diameter();
}

double FundamentalDomainGenerator::squareDiameterNorm(const H2Polygon &polygon)
{
    double diameter = polygon.diameter();
    return diameter*diameter;
}



double FundamentalDomainGenerator::minAngleNorm(const H2Polygon &polygon)
{
    return 2*M_PI - polygon.smallestAngle();
}

double FundamentalDomainGenerator::maxAngleNorm(const H2Polygon &polygon)
{
    std::vector<double> angles = polygon.getInteriorAngles();
    return *std::max_element(angles.begin(), angles.end());
}

double FundamentalDomainGenerator::minDistanceToNonNeighborSideNorm(const H2Polygon &polygon)
{
    std::vector<H2GeodesicArc> sides = polygon.getSides();
    std::vector<double> sideDistances, sideLengths;
    for (const auto & side : sides)
    {
        for (const auto & otherSide : sides)
        {
            if (!H2GeodesicArc::shareEndpoint(side,otherSide))
            {
                sideDistances.push_back(H2Geodesic::distanceGeodesics(side.getGeodesic(),otherSide.getGeodesic()));
            }
        }
        sideLengths.push_back(side.length());
    }

    return -*std::min_element(sideDistances.begin(),sideDistances.end());
}

double FundamentalDomainGenerator::energyNorm(const H2Polygon &polygon)
{
    std::vector<H2Point> vertices = polygon.getVertices();

    uint i, j, N = vertices.size();

    double sum = 0, distance;
    for (i=0; i+1!=N; ++i)
    {
        for (j=i+1; j!=N; ++j)
        {
            distance = H2Point::distance(vertices[i], vertices[j]);
            sum += distance*distance;
        }
    }

    return sum;
}

double FundamentalDomainGenerator::distanceToIdealBoundaryNorm(const H2Polygon &polygon)
{
    std::vector<H2Point> vertices = polygon.getVertices();
    std::vector<double> distances;
    distances.reserve(vertices.size());

    for (const auto &vertex : vertices)
    {
        distances.push_back(norm(vertex.getDiskCoordinate()));
    }

    return *std::max_element(distances.begin(), distances.end());
}

double FundamentalDomainGenerator::isoperimetricNorm(const H2Polygon &polygon)
{
    std::vector<H2GeodesicArc> sides = polygon.getSides();

    double sum = 0;
    for (const auto &side : sides)
    {
        sum += side.length();
    }

    return sum*sum;
}


H2Point FundamentalDomainGenerator::lineSearch(const H2Point &x0, const Complex &u, double t0) const
{
    double tol1 = tol;
    double error = 1 + tol1;

    double tk = t0, previous = t0;
    double PhiL, PhiC, PhiR;
    uint counter = 0;

    while((error>tol1) && (counter!=maxLineSearchIterations))
    {
        PhiL = Phi(x0, u, previous  - delta);
        PhiC = Phi(x0, u, previous);
        PhiR = Phi(x0, u, previous + delta);

        if ((std::abs((PhiR - PhiL)/(2))<tol1) || (std::abs((PhiR + PhiL - 2*PhiC)/(delta)) < tol1))
        {
            break;
        }

        tk = previous - (1/2)*(PhiR - PhiL)/((PhiR + PhiL - 2*PhiC)/delta);
        error = std::abs(tk - previous);
        previous = tk;
        ++counter;
    }
    return H2Point::exponentialMap(x0, u, tk);
}

H2Point FundamentalDomainGenerator::optimalStepGradientDescent(const H2Point &x0) const
{
    clock_t start = clock();

    H2Point xk = x0, previous = x0;

    double error = 1 + tol;
    uint counter = 0;
    Complex z, gradFk;
    double g, gradLength;

    while((error>tol) && (counter!=maxIterations))
    {
        z = previous.getDiskCoordinate();
        g = 4/((1 - norm(z))*(1 - norm(z)));
        gradFk = gradF(previous);
        gradLength = sqrt(g*norm(gradFk));
        if (gradLength < tol)
        {
            break;
        }
        xk = lineSearch(previous, -gradFk, gradLength);
        error = H2Point::distance(xk, previous);
        previous = xk;
        ++ counter;
    }

    if (counter==maxIterations)
    {
        qDebug() << "Warning in FundamentalDomainGenerator::optimalStepGradientDescent: optimalStepGradientDescent failed, final error: " << error;
    }
    else
    {
        std::cout << "Success of optimalStepGradientDescent in " << counter << " iterations" << std::endl;
    }

    std::cout<< "Time elapsed: " << (clock()-start)*1.0/CLOCKS_PER_SEC << std::endl;

    return xk;
}
