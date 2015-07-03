#ifndef FUNDAMENTALDOMAINGENERATOR_H
#define FUNDAMENTALDOMAINGENERATOR_H

#include "tools.h"

#include "grouprepresentation.h"


class FundamentalDomainGenerator
{
public:
    FundamentalDomainGenerator(const GroupRepresentation<H2Isometry> &rho);
    FundamentalDomainGenerator() = delete;

    H2Polygon getOptimalFundamentalDomain() const;


private:
    double F(const H2Point &x) const;

    double Phi(const H2Point &x0, const Complex &u, const double &t) const;
    Complex gradF(const H2Point &x) const;
    double dPhi(const H2Point &x0, const Complex &u, const double &t) const;

    static double diameterNorm(const H2Polygon &polygon);
    static double squareDiameterNorm(const H2Polygon &polygon);
    static double minAngleNorm(const H2Polygon &polygon);
    static double maxAngleNorm(const H2Polygon &polygon);
    static double minDistanceToNonNeighborSideNorm(const H2Polygon &polygon);
    static double energyNorm(const H2Polygon &polygon);
    static double distanceToIdealBoundaryNorm(const H2Polygon &polygon);
    static double isoperimetricNorm(const H2Polygon &polygon);

    H2Point lineSearch(const H2Point &x0, const Complex &u, double t0) const;
    H2Point optimalStepGradientDescent(const H2Point &x0) const;


    H2Point xInit;

    double tol;
    double delta;
    uint maxIterations;
    uint maxLineSearchIterations;

    Complex delX, delY;

    GroupRepresentation<H2Isometry> rho;
};

#endif // FUNDAMENTALDOMAINGENERATOR_H
