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
    explicit DiscreteGroup(const TopologicalSurface &S);

    std::vector<generatorName> getGenerators() const;
    std::vector<Word> getRelations() const;
    std::vector<Word> getCusps() const;
    uint numberOfCusps() const;

    std::vector<Word> getWordsOfLengthLessThan(uint n) const;
    std::vector<Word> getWordsOfLengthEqualTo(uint n) const;
    std::vector<Word> getWordsOfNonRepeatingLettersLengthLessThan(uint n) const;
    std::vector<Word> getWordsOfNonRepeatingLettersLengthEqualTo(uint n) const;

    std::vector<Word> getSidePairings() const;
    std::vector<Word> getPairingsFromVertex() const;
    std::vector<Word> getPairingsAroundVertex() const;
    std::vector<Word> getPairingsAroundVertices() const;

    std::string getWordAsString(const Word & w) const;
    std::string getLetterAsString(const letter & l) const;
    bool isClosedSurfaceGroup() const;

    void setPairOfPants(generatorName c1, generatorName c2, generatorName c3);
    void rotateGenerators(int rightshift);

    static DiscreteGroup amalgamateOverInverse(const DiscreteGroup & Gamma1, const generatorName &a1,
                                               const DiscreteGroup & Gamma2, const generatorName &a1inverse);

    static DiscreteGroup HNNextensionOverInverse(const DiscreteGroup & Gamma, const generatorName &a, const generatorName &ainverse,
                                                 const generatorName & newGeneratorName);

private:
    void reset();
    void rotateWord(Word &w, int shift);
    static bool checkCompatibilityForAmalgamation(const DiscreteGroup &Gamma1, const DiscreteGroup &Gamma2);
    static bool checkCompatibilityforHNNextension(const DiscreteGroup &Gamma);
    bool findGeneratorIndex(uint &outputIndex, const generatorName &a) const;

    std::vector<generatorName> generators;
    std::vector<Word> relations;
    std::vector<Word> cusps;
    bool closedSurfaceGroup;
};


#endif // DISCRETEGROUP_H
