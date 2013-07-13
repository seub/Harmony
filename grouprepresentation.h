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
    H2Polygon generateFundamentalDomainForNormalizedSurfaceGroup(const H2Point & basePoint) const;
    std::vector<H2Isometry> getSidePairingsForNormalizedFundamentalDomain() const;



    void setFenchelNielsenCoordinates(const std::vector<double> & lengths, const std::vector<double> & twists);

    void setNormalizedPairOfPantsRepresentation(generatorName c1, generatorName c2, generatorName c3,
                                                double length1, double length2, double length3, generatorName normalized);

    static IsomH2Representation amalgamateOverInverse(DiscreteGroup *outputDiscreteGroup,
                                                      const IsomH2Representation & rho1, const generatorName &a1,
                                                      const generatorName &a1left,
                                                      const IsomH2Representation & rho2,
                                                      const generatorName &a1inverse,
                                                      const generatorName &a1inverseleft,
                                                      double twist);
    static IsomH2Representation amalgamateOverInverse(DiscreteGroup *outputDiscreteGroup,
                                                      const IsomH2Representation & rho1, const generatorName &a1,
                                                      const IsomH2Representation & rho2,
                                                      const generatorName &a1inverse,
                                                      const H2Isometry & conjugator);
    static IsomH2Representation amalgamateOverInverse(DiscreteGroup *outputDiscreteGroup,
                                                      const IsomH2Representation & rho1, const generatorName &a1,
                                                      const IsomH2Representation & rho2,
                                                      const generatorName &a1inverse);
    static IsomH2Representation doHNNextensionOverInverse(DiscreteGroup *outputDiscreteGroup, const IsomH2Representation &rho,
                                                          const generatorName &a, const generatorName &aleft,
                                                          const generatorName &ainverse, const generatorName &ainverseleft,
                                                          const generatorName &newGeneratorName, double twist);
    static IsomH2Representation doHNNextensionOverInverse(DiscreteGroup *outputDiscreteGroup, const IsomH2Representation &rho,
                                                          const generatorName &a,
                                                          const generatorName &ainverse,
                                                          const generatorName &newGeneratorName,
                                                          const H2Isometry & conjugator);


    //private:
    DiscreteGroup* Gamma;
    std::vector<T> generatorImages;

    static IsomH2Representation setFNCoordinatesUnnormalized(DiscreteGroup *outputDiscreteGroup, const std::vector<double> & lengths, const std::vector<double> & twists);
    static IsomH2Representation setFNCoordinatesUnnormalizedNew(DiscreteGroup *outputDiscreteGroup, const std::vector<double> & lengths, const std::vector<double> & twists);
    static bool checkCompatibilityOfFNcoordinates(const std::vector<double> & lengths, const std::vector<double> & twists);
    static std::vector<IsomH2Representation> getPantsRepresentations(const std::vector<double> & lengths,
                                                                     const std::vector<double> & twists,
                                                                     const std::vector<DiscreteGroup*> & outputDiscreteGroups);
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
