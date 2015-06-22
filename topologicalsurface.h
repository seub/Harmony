#ifndef TOPOLOGICALSURFACE_H
#define TOPOLOGICALSURFACE_H
#include "types.h"

class DiscreteGroup;

class TopologicalSurface
{
public:
    TopologicalSurface();
    TopologicalSurface(uint genus, uint numberOfPunctures);

    uint getGenus() const;
    uint getNumberOfPunctures() const;
    bool isClosedHyperbolicSurface() const;

    std::vector<DiscreteGroup> getPantsDecomposition() const;

private:
    uint genus;
    uint numberOfPunctures;
};

#endif // TOPOLOGICALSURFACE_H
