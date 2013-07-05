#ifndef DISCRETEGROUP_H
#define DISCRETEGROUP_H

#include "types.h"
#include "topologicalsurface.h"

class DiscreteGroup
{
    friend std::ostream & operator<<(std::ostream &out, const DiscreteGroup & Gamma);

public:
    DiscreteGroup();
    DiscreteGroup(const TopologicalSurface & S);

    std::vector<generatorName> getGenerators() const;
    std::vector<word> getRelations() const;

    std::string getWordString(const word & w) const;
    std::string getLetterString(const letter & l) const;

private:
    std::vector<generatorName> generators;
    std::vector<word> relations;
    std::vector<word> cusps;
};




#endif // DISCRETEGROUP_H
