#ifndef MATHSCONTAINER_H
#define MATHSCONTAINER_H

#include "tools.h"
#include "topfactory.h"
#include "liftedgraph.h"
#include "grouprepresentation.h"

class MathsContainer
{
    friend class ActionHandler;
    friend class TopFactory;

public:
    MathsContainer() {}

private:
    GroupRepresentation<H2Isometry> rhoDomain;
    GroupRepresentation<H2Isometry> H2RhoImage;
    LiftedGraphFunctionTriangulated<H2Point, H2Isometry> domainFunction;
    LiftedGraphFunctionTriangulated<H2Point, H2Isometry> H2ImageFunction;
};

#endif // MATHSCONTAINER_H
