#ifndef SURFACEGROUPREPRESENTATION_H
#define SURFACEGROUPREPRESENTATION_H

#include "types.h"
#include "discretegroup.h"
#include "sl2cmatrix.h"

class GroupRepresentation
{    

public:

    GroupRepresentation();
    GroupRepresentation(DiscreteGroup* Gamma);
    GroupRepresentation(DiscreteGroup* Gamma, std::vector<SL2Cmatrix> listOfMatrices);


    bool checkRelations() const;
    SL2Cmatrix evaluateRepresentation(const word & w) const;

    GroupRepresentation conj(const SL2Cmatrix & A);

    GroupRepresentation conjugate() const;

private:
    DiscreteGroup* Gamma;
    std::vector<SL2Cmatrix> listOfMatrices;
};

#endif // SURFACEGROUPREPRESENTATION_H
