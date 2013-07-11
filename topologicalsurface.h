#ifndef TOPOLOGICALSURFACE_H
#define TOPOLOGICALSURFACE_H
#include "types.h"

class DiscreteGroup;

class TopologicalSurface
{
public:
    TopologicalSurface();
    TopologicalSurface(int genus, int numberOfPunctures);

    int getGenus() const;
    int getNumberOfPunctures() const;

    std::vector<DiscreteGroup> getPantsDecomposition() const;


private:
    int genus;
    int numberOfPunctures;
};

#endif // TOPOLOGICALSURFACE_H
