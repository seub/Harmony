#ifndef GROUPREPRESENTATION_H
#define GROUPREPRESENTATION_H

#include "types.h"
#include "discretegroup.h"
#include "sl2cmatrix.h"

class GroupRepresentation
{    

public:

    GroupRepresentation();
    GroupRepresentation(DiscreteGroup* Gamma);
    GroupRepresentation(DiscreteGroup* Gamma, std::vector<SL2CMatrix> listOfMatrices);


    bool checkRelations() const;
    SL2CMatrix evaluateRepresentation(const word & w) const;

    GroupRepresentation conj(const SL2CMatrix & A);

    GroupRepresentation conjugate() const;

private:
    DiscreteGroup* Gamma;
    std::vector<SL2CMatrix> listOfMatrices;
};

#endif // GROUPREPRESENTATION_H
