#ifndef MATHSCONTAINER_H
#define MATHSCONTAINER_H

#include "tools.h"
#include "equivariantharmonicmapsfactory.h"

class MathsContainer
{
    friend class ActionHandler;

public:
    MathsContainer();

private:
    EquivariantHarmonicMapsFactory factory;
};

#endif // MATHSCONTAINER_H
