#ifndef GROUPREPRESENTATION_H
#define GROUPREPRESENTATION_H

#include "types.h"
#include "discretegroup.h"
#include "sl2cmatrix.h"
#include "sl2rmatrix.h"
#include "h2polygon.h"


template <typename T> class GroupRepresentation
{    

public:

    GroupRepresentation();
    GroupRepresentation(DiscreteGroup* Gamma);
    GroupRepresentation(DiscreteGroup* Gamma, std::vector<T> listOfMatrices);


    bool checkRelations() const;
    T evaluateRepresentation(const word & w) const;


    GroupRepresentation<T> conj(const T & A);
    GroupRepresentation<T> conjugate() const;

    //Specialization to GroupRepresentation<SL2RMatrix> (i.e. FuchsianRepresentation)
    H2Polygon generatePolygon(const H2Point & basePoint) const;
    void setFenchelNielsenCoordinates(const std::vector<double> & lengthParameters, const std::vector<double> & twistParameters);

private:
    DiscreteGroup* Gamma;
    std::vector<T> listOfMatrices;
};

#endif // GROUPREPRESENTATION_H
