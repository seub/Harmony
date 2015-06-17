#ifndef MATHSCONTAINER_H
#define MATHSCONTAINER_H

#include "tools.h"
#include "topfactory.h"

class MathsContainer
{
    friend class ActionHandler;

public:
    MathsContainer();

private:
    TopFactory topFactory;
};

#endif // MATHSCONTAINER_H
