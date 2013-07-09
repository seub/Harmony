#ifndef DISCRETEGROUP_H
#define DISCRETEGROUP_H

#include "types.h"
#include "topologicalsurface.h"

class DiscreteGroup
{
    friend std::ostream & operator<<(std::ostream &out, const DiscreteGroup & Gamma);


public:
    DiscreteGroup();
    DiscreteGroup(const std::vector<generatorName> & generators, const std::vector<word> &relations);
    DiscreteGroup(const TopologicalSurface & S);

    std::vector<generatorName> getGenerators() const;
    std::vector<word> getRelations() const;
    std::vector<word> getCusps() const;
    int numberOfCusps() const;

    std::string getWordAsString(const word & w) const;
    std::string getLetterAsString(const letter & l) const;
    bool isClosedSurfaceGroup() const;

    void setPairOfPants(generatorName c1, generatorName c2, generatorName c3);
    void rotateGenerators(int shift);

private:
    std::vector<generatorName> generators;
    std::vector<word> relations;
    std::vector<word> cusps;
    bool closedSurfaceGroup;

    void reset();
    void rotateWord(word & w, int shift);
};

bool findUniqueCommonGenerator(const DiscreteGroup & Gamma1, const DiscreteGroup & Gamma2, generatorName &outputName,
                               int & outputIndex1, int & outputIndex2);
DiscreteGroup amalgamateOverCommonGenerator(const DiscreteGroup & Gamma1, const DiscreteGroup & Gamma2);

#endif // DISCRETEGROUP_H
