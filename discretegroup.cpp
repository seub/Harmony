#include "discretegroup.h"
#include "tools.h"

DiscreteGroup::DiscreteGroup()
{
    closedSurfaceGroup = false;
}

DiscreteGroup::DiscreteGroup(const std::vector<generatorName> &generators, const std::vector<word> &relations)
{
    closedSurfaceGroup = false;
    this->generators = generators;
    this->relations = relations;
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

std::vector<word> DiscreteGroup::getCusps() const
{
    return cusps;
}

int DiscreteGroup::numberOfCusps() const
{
    return cusps.size();
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


bool DiscreteGroup::checkCompatibilityForAmalgamation(const DiscreteGroup &Gamma1, const DiscreteGroup &Gamma2)
{
    std::vector<generatorName> generators1 = Gamma1.getGenerators();
    std::vector<generatorName> generators2 = Gamma2.getGenerators();

    if (containsDuplicates(generators1) || containsDuplicates(generators2))
    {
        std::cout << "WARNING in DiscreteGroup::checkCompatibilityForAmalgamation: group generators contains duplicates!" << std::endl;
        return false;
    }
    if (haveCommonElements(generators1, generators2))
    {
        std::cout << "WARNING in DiscreteGroup::checkCompatibilityForAmalgamation: common generator names!" << std::endl;
        return false;
    }
    if (Gamma1.numberOfCusps() != 0 || Gamma2.numberOfCusps() != 0)
    {
        std::cout << "WARNING in DiscreteGroup::checkCompatibilityForAmalgamation: I don't handle cusps (for the moment)!" << std::endl;
        return false;
    }
    return true;
}

bool DiscreteGroup::findGeneratorIndex(int &outputIndex, const generatorName &a) const
{
    for (unsigned int i=0; i<generators.size(); i++)
    {
        if (generators[i]==a)
        {
            outputIndex = i;
            return true;
        }
    }
    return false;
}

DiscreteGroup DiscreteGroup::amalgamateOverInverse(const DiscreteGroup &Gamma1, const generatorName &a1,
                                                   const DiscreteGroup &Gamma2, const generatorName &a1inverse,
                                                   const generatorName &newGeneratorName)
{
    DiscreteGroup::checkCompatibilityForAmalgamation(Gamma1, Gamma2);
    int i1, j1;
    if (!Gamma1.findGeneratorIndex(i1, a1) || !Gamma2.findGeneratorIndex(j1, a1inverse))
    {
        std::cout << "WARNING in DiscreteGroup::amalgamateOverInverse: generator is not in the group!" << std::endl;
    }

    std::vector<generatorName> outputGenerators;
    std::vector<word> outputRelations;

    if(checkCompatibilityForAmalgamation(Gamma1, Gamma2))
    {
        std::vector<generatorName> generators1 = Gamma1.getGenerators();
        std::vector<generatorName> generators2 = Gamma2.getGenerators();
        std::vector<word> relations1 = Gamma1.getRelations();
        std::vector<word> relations2 = Gamma2.getRelations();

        int i, j;
        for (i=0; i< (int) generators1.size(); i++)
        {
            if (i == i1)
            {
                outputGenerators.push_back(newGeneratorName);
            }
                else
            {
                outputGenerators.push_back(generators1[i]);
            }
        }
        for (j=0; j< (int) generators2.size(); j++)
        {
            if (j != j1)
            {
                outputGenerators.push_back(generators2[j]);
            }
        }

        word w;
        outputRelations = relations1;

        for (i=0; i< (int) relations2.size(); i++)
        {
            w = relations2[i];
            for (j=0; j< (int) w.size(); j++)
            {
                if (w[j].first == j1)
                {
                    w[j].first = i1;
                    w[j].second = - w[j].second;
                }
                else if(w[j].first < j1)
                {
                    w[j].first += generators1.size();
                }
                else if(w[j].first > j1)
                {
                    w[j].first += generators1.size() - 1;
                }
            }
            outputRelations.push_back(w);
        }
    }

    return DiscreteGroup(outputGenerators, outputRelations);
}

void DiscreteGroup::reset()
{
    generators.clear();
    relations.clear();
    cusps.clear();
    closedSurfaceGroup = false;
}

void DiscreteGroup::setPairOfPants(generatorName c1, generatorName c2, generatorName c3)
{
    reset();
    generators.push_back(c1);
    generators.push_back(c2);
    generators.push_back(c3);
    word w;
    w.push_back(letter(2, 1));
    w.push_back(letter(1, 1));
    w.push_back(letter(0, 1));
    relations.push_back(w);
    return;
}

void DiscreteGroup::rotateGenerators(int shift)
{
    int antishift = generators.size() - shift;
    rotate(generators.begin(), generators.begin() + antishift, generators.end());

    /*for (unsigned int i=0; i<relations.size(); i++)
    {
        rotateWord(relations[i], shift);
    }
    for(unsigned int j=0; j<cusps.size(); j++)
    {
        rotateWord(cusps[j], shift);
    }*/
    return;
}

void DiscreteGroup::rotateWord(word &w, int shift)
{
    int n = generators.size();
    for (unsigned int i=0; i<w.size(); i++)
    {
        w[i].first = (w[i].first + shift) % n;
    }
    return;
}
