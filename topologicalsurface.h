#ifndef TOPOLOGICALSURFACE_H
#define TOPOLOGICALSURFACE_H


#include "tools.h"

class DiscreteGroup;

class TopologicalSurface
{
public:
    TopologicalSurface() = delete;
    TopologicalSurface(uint genus, uint nbPunctures);

    int eulerCharacteristic() const;
    bool isHyperbolic() const;
    uint getGenus() const;
    uint getNumberOfPunctures() const;
    bool isClosedHyperbolicSurface() const;

    std::vector<DiscreteGroup> getPantsDecomposition() const;
    bool maxInjectivityRadius(double &out) const;

private:
    uint genus;
    uint nbPunctures;
};

class maxInjectivityRadiusCalculator
{
    friend class TopologicalSurface;

private:
    maxInjectivityRadiusCalculator(const TopologicalSurface &surface);
    maxInjectivityRadiusCalculator(const maxInjectivityRadiusCalculator&);
    maxInjectivityRadiusCalculator operator=(maxInjectivityRadiusCalculator);

    bool get(double &out, int maxNbIterations = 20) const;

    double f(const double &x) const;
    double df(const double &x) const;
    double x0() const;
    double newtonIterate(const double &xk) const;

    uint g, n;
    double error, tol;
};

#endif // TOPOLOGICALSURFACE_H
