#ifndef DISCRETEGROUP_H
#define DISCRETEGROUP_H

#include "tools.h"
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

    std::vector<word> getWordsOfLengthLessThan(int n) const;
    std::vector<word> getWordsOfLengthEqualTo(int n) const;
    std::vector<word> getWordsOfNonRepeatingLettersLengthLessThan(int n) const;
    std::vector<word> getWordsOfNonRepeatingLettersLengthEqualTo(int n) const;

    std::string getWordAsString(const word & w) const;
    std::string getLetterAsString(const letter & l) const;
    bool isClosedSurfaceGroup() const;

    void setPairOfPants(generatorName c1, generatorName c2, generatorName c3);
    void rotateGenerators(int shift);

    static DiscreteGroup amalgamateOverInverse(const DiscreteGroup & Gamma1, const generatorName &a1,
                                               const DiscreteGroup & Gamma2, const generatorName &a1inverse);

    static DiscreteGroup doHNNextensionOverInverse(const DiscreteGroup & Gamma, const generatorName &a, const generatorName &ainverse,
                                                   const generatorName & newGeneratorName);

private:
    std::vector<generatorName> generators;
    std::vector<word> relations;
    std::vector<word> cusps;
    bool closedSurfaceGroup;

    void reset();
    void rotateWord(word & w, int shift);
    static bool checkCompatibilityForAmalgamation(const DiscreteGroup & Gamma1, const DiscreteGroup & Gamma2);
    static bool checkCompatibilityforHNNextension(const DiscreteGroup & Gamma);
    bool findGeneratorIndex(int & outputIndex, const generatorName &a) const;
};


#endif // DISCRETEGROUP_H
