#include "discretegroup.h"

DiscreteGroup::DiscreteGroup()
{
    closedSurfaceGroup = false;
}

DiscreteGroup::DiscreteGroup(const TopologicalSurface &S)
{
    int genus = S.getGenus();
    int numberOfPunctures = S.getNumberOfPunctures();
    cusps.resize(numberOfPunctures);

    std::vector<generatorName> generatorsList;
    generatorsList.reserve(2*genus);

    int i;
    for (i=1; i<=genus; i++)
    {
        std::string s;
        std::stringstream out;
        out << i;
        s = out.str();

        std::string a_i;
        a_i.append("a");
        a_i.append(s);
        std::string b_i;
        b_i.append("b");
        b_i.append(s);
        generatorsList.push_back(a_i);
        generatorsList.push_back(b_i);
    }

    int j;
    for(j=1; j<=numberOfPunctures; j++)
    {
        std::string s;
        std::stringstream out;
        out << j;
        s = out.str();

        std::string c_j;
        c_j.append("c");
        c_j.append(s);
        generatorsList.push_back(c_j);

    }

    generators = generatorsList;

    std::vector<word> relationsList;

    std::vector<generatorName> gen = generatorsList;

    std::vector<letter> relation;
    letter l;
    int k;
    for (k=0;k<genus;k++)
    {
        l.first = 2*k;
        l.second = 1;
        relation.push_back(l);
        l.first = 2*k+1;
        l.second = 1;
        relation.push_back(l);
        l.first = 2*k;
        l.second = -1;
        relation.push_back(l);
        l.first = 2*k+1;
        l.second = -1;
        relation.push_back(l);
    }
    for (k=2*genus;k<2*genus+numberOfPunctures;k++)
    {
        l.first = k;
        l.second = 1;
        relation.push_back(l);
        (cusps[k-2*genus]).clear();
        (cusps[k-2*genus]).push_back(l);
    }
    relationsList.push_back(relation);

    relations = relationsList;

    closedSurfaceGroup = (numberOfPunctures == 0);
}


std::vector<generatorName> DiscreteGroup::getGenerators() const
{
    return generators;
}

bool DiscreteGroup::isClosedSurfaceGroup() const
{
    return closedSurfaceGroup;
}

std::vector<word> DiscreteGroup::getRelations() const
{
    return relations;
}


std::ostream & operator<<(std::ostream &out, const DiscreteGroup & Gamma)
{
    out << "< ";
    std::vector<generatorName> generators = Gamma.generators;
    out << generators[0];
    for(unsigned int i=1; i<generators.size(); i++)
    {
        out << ", " << generators[i];
    }
    out << " | ";
    std::vector<word> relations = Gamma.relations;
    out << Gamma.getWordAsString(relations[0]);
    for(unsigned int i=1; i<relations.size(); i++)
    {
        out << ", " << Gamma.getWordAsString(relations[i]);
    }
    out << " >" << std::endl;

    return out;
}


std::string DiscreteGroup::getLetterAsString(const letter & l) const
{
    std::string res;
    res = generators[l.first];
    if (l.second!=1)
    {
        std::stringstream ss;
        ss << l.second;
        res.append("^").append(ss.str());
    }
    return res;
}


std::string DiscreteGroup::getWordAsString(const word & w) const
{
    std::string res;
    unsigned int i;
    for(i=0;i<w.size();i++)
    {
        res.append(getLetterAsString(w[i])).append(" ");
    }
    return res;
}

