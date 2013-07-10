#ifndef GROUPREPRESENTATION_H
#define GROUPREPRESENTATION_H

#include "tools.h"
#include "discretegroup.h"
#include "sl2cmatrix.h"
#include "sl2rmatrix.h"
#include "h2polygon.h"
#include "h2isometry.h"
#include "h3isometry.h"


template <typename T> class GroupRepresentation
{    
    template<typename T2> friend std::ostream & operator<<(std::ostream & out, const GroupRepresentation<T2> & rho);

public:
    GroupRepresentation(DiscreteGroup* Gamma);
    GroupRepresentation(DiscreteGroup* Gamma, std::vector<T> generatorImages);


    bool checkRelations() const;
    T evaluateRepresentation(const word & w) const;
    std::vector<T> getGeneratorImages() const;
    bool getGeneratorImage(const generatorName &a, T &output) const;
    void rotateGenerators(int shift);
    DiscreteGroup getDiscreteGroup() const;



    GroupRepresentation<T> conj(const T & A) const;



    // Specialization to GroupRepresentation<SL2CMatrix>
    GroupRepresentation<SL2CMatrix> conjugate() const;

    //Specialization to GroupRepresentation<H2ISometry> (i.e. IsomH2Representation)
    H2Polygon generatePolygon(const H2Point & basePoint) const;
    void setFenchelNielsenCoordinates(const std::vector<double> & lengthParameters, const std::vector<double> & twistParameters);
    void setNormalizedPairOfPantsRepresentation(generatorName c1, generatorName c2, generatorName c3,
                                                double length1, double length2, double length3, generatorName normalized);

    static IsomH2Representation amalgamateOverInverse(DiscreteGroup *outputDiscreteGroup,
                                                       const IsomH2Representation & rho1, const generatorName &a1, const generatorName &a1left,
                                               const IsomH2Representation & rho2, const generatorName &a1inverse, const generatorName &a1inverseleft,
                                               const generatorName &newGeneratorName, double twist);
    static IsomH2Representation doHNNextensionOverInverse(DiscreteGroup *outputDiscreteGroup, const IsomH2Representation &rho,
                                                          const generatorName &a, const generatorName &aleft,
                                                          const generatorName &ainverse, const generatorName &ainverseleft,
                                                          const generatorName &newGeneratorName, double twist);



    private:
    DiscreteGroup* Gamma;
    std::vector<T> generatorImages;
};

template<typename T> std::ostream & operator<<(std::ostream & out, const GroupRepresentation<T> & rho)
{
    std::vector<generatorName> generators = rho.Gamma->getGenerators();
    out << std::endl;
    out << "Group representation from the discrete group:" << std::endl;
    out << *rho.Gamma;
    out << "Images of the generators:" << std::endl;
    for (unsigned int i=0; i<generators.size(); i++)
    {
        out << generators[i] << " -> " << rho.generatorImages[i] << std::endl;
    }
    return out;
}

#endif // GROUPREPRESENTATION_H
