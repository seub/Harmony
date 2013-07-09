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
    GroupRepresentation(DiscreteGroup* Gamma);
    GroupRepresentation(DiscreteGroup* Gamma, std::vector<T> listOfMatrices);


    bool checkRelations() const;
    T evaluateRepresentation(const word & w) const;


    GroupRepresentation<T> conj(const T & A);
    GroupRepresentation<T> conjugate() const;

    //Specialization to GroupRepresentation<H2ISometry> (i.e. IsomH2Representation)
    H2Polygon generatePolygon(const H2Point & basePoint) const;
    void setFenchelNielsenCoordinates(const std::vector<double> & lengthParameters, const std::vector<double> & twistParameters);
    void setNormalizedPairOfPantsRepresentation(generatorName c1, generatorName c2, generatorName c3,
                                                double length1, double length2, double length3, generatorName normalized);
    void rotateGenerators(int shift);


private:
    DiscreteGroup* Gamma;
    std::vector<T> listOfMatrices;
};

#endif // GROUPREPRESENTATION_H
