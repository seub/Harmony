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
    template<typename T2> friend bool operator ==(const GroupRepresentation<T2> &rho1, const GroupRepresentation<T2> &rho2);
    friend class FenchelNielsenConstructor;

public:
    GroupRepresentation();
    explicit GroupRepresentation(const DiscreteGroup &Gamma);
    GroupRepresentation(const DiscreteGroup &Gamma, std::vector<T> generatorImages);


    bool checkRelations() const;
    std::vector<T> getGeneratorImages() const;
    bool getGeneratorImage(const generatorName &a, T &output) const;
    DiscreteGroup getDiscreteGroup() const;

    T evaluateRepresentation(const Word & w) const;
    std::vector<T> evaluateRepresentation(const std::vector<Word> & listOfWords) const;

    void rotateGenerators(int shift);
    GroupRepresentation<T> conjugate(const T &conjugator) const;

    // Specialization to GroupRepresentation<SL2CMatrix>
    GroupRepresentation<SL2CMatrix> bar() const;

    //Specialization to GroupRepresentation<H2ISometry>
    std::vector<H2Isometry> getSidePairings() const;
    std::vector<H2Isometry> getPairingsFromVertex() const;
    std::vector<H2Isometry> getPairingsAroundVertex() const;
    std::vector<H2Isometry> getPairingsAroundVertices() const;

    H2Polygon getFundamentalDomain(const H2Point &firstVertex) const;
    H2Polygon getOptimalFundamentalDomain() const;

    void setNormalizedPairOfPantsRepresentation(generatorName c1, generatorName c2, generatorName c3,
                                                double length1, double length2, double length3, generatorName normalized);

    void setNormalizedPairOfPantsRepresentation(generatorName c1, generatorName c2, generatorName c3,
                                                double C1, double C2, double C3,
                                                double S1, double S2, double S3,
                                                generatorName normalized);
    void setNiceRepresentation();





    static GroupRepresentation<H2Isometry> amalgamateOverInverse(const GroupRepresentation<H2Isometry> & rho1, const generatorName &a1,
                                                      const GroupRepresentation<H2Isometry> & rho2,
                                                      const generatorName &a1inverse);

    static GroupRepresentation<H2Isometry> HNNextensionOverInverse(const GroupRepresentation<H2Isometry> &rho,
                                                          const generatorName &a,
                                                          const generatorName &ainverse,
                                                          const generatorName &newGeneratorName,
                                                          const H2Isometry & conjugator);


private:
    DiscreteGroup Gamma;
    std::vector<T> generatorImages;

    static bool checkCompatibilityOfFNcoordinates(const std::vector<double> & lengths, const std::vector<double> & twists);

};



template<typename T> std::ostream & operator<<(std::ostream & out, const GroupRepresentation<T> & rho)
{
    std::vector<generatorName> generators = rho.Gamma.getGenerators();
    out << std::endl;
    out << "Group representation from the discrete group:" << std::endl;
    out << rho.Gamma;
    out << "Images of the generators:" << std::endl;
    for (uint i=0; i != generators.size(); ++i)
    {
        out << generators[i] << " -> " << rho.generatorImages[i] << std::endl;
    }
    return out;
}

template<typename T2> bool operator==(const GroupRepresentation<T2> &rho1, const GroupRepresentation<T2> &rho2)
{
    std::vector<T2> generatorImages1, generatorImages2;
    generatorImages1 = rho1.getGeneratorImages();
    generatorImages2 = rho2.getGeneratorImages();
    bool output = 1;
    int j=0;
    while (output && j<(int)generatorImages1.size())
    {
        output = generatorImages1[j]==generatorImages2[j];
        ++j;
    }
    return output;
}

#endif // GROUPREPRESENTATION_H
