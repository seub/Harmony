#ifndef DISCRETEGROUP_H
#define DISCRETEGROUP_H

#include "tools.h"
#include "word.h"
#include "topologicalsurface.h"

class DiscreteGroup
{
    friend std::ostream & operator<<(std::ostream &out, const DiscreteGroup & Gamma);


public:
    DiscreteGroup();
    DiscreteGroup(const std::vector<generatorName> &generators, const std::vector<Word> &relations);
    DiscreteGroup(const TopologicalSurface &S);

    std::vector<generatorName> getGenerators() const;
    std::vector<Word> getRelations() const;
    std::vector<Word> getCusps() const;
    int numberOfCusps() const;

    std::vector<Word> getWordsOfLengthLessThan(int n) const;
    std::vector<Word> getWordsOfLengthEqualTo(int n) const;
    std::vector<Word> getWordsOfNonRepeatingLettersLengthLessThan(int n) const;
    std::vector<Word> getWordsOfNonRepeatingLettersLengthEqualTo(int n) const;

    std::string getWordAsString(const Word & w) const;
    std::string getLetterAsString(const letter & l) const;
    bool isClosedSurfaceGroup() const;

    void setPairOfPants(generatorName c1, generatorName c2, generatorName c3);
    void rotateGenerators(int rightshift);

    static DiscreteGroup amalgamateOverInverse(const DiscreteGroup & Gamma1, const generatorName &a1,
                                               const DiscreteGroup & Gamma2, const generatorName &a1inverse);

    static DiscreteGroup doHNNextensionOverInverse(const DiscreteGroup & Gamma, const generatorName &a, const generatorName &ainverse,
                                                   const generatorName & newGeneratorName);

private:
    std::vector<generatorName> generators;
    std::vector<Word> relations;
    std::vector<Word> cusps;
    bool closedSurfaceGroup;

    void reset();
    void rotateWord(Word &w, int shift);
    static bool checkCompatibilityForAmalgamation(const DiscreteGroup &Gamma1, const DiscreteGroup &Gamma2);
    static bool checkCompatibilityforHNNextension(const DiscreteGroup &Gamma);
    bool findGeneratorIndex(int & outputIndex, const generatorName &a) const;
};


#endif // DISCRETEGROUP_H
