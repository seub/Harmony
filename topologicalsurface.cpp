#include "topologicalsurface.h"

TopologicalSurface::TopologicalSurface()
{
}

TopologicalSurface::TopologicalSurface(int genus, int numberOfPunctures) : genus(genus), numberOfPunctures(numberOfPunctures)
{
}

int TopologicalSurface::getGenus() const
{
    return genus;
}

int TopologicalSurface::getNumberOfPunctures() const
{
    return numberOfPunctures;
}
