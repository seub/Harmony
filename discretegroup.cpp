#include "discretegroup.h"

DiscreteGroup::DiscreteGroup()
{
    closedSurfaceGroup = false;
}

DiscreteGroup::DiscreteGroup(const std::vector<generatorName> &generators, const std::vector<Word> &relations)
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

    std::string s;
    for (int i=1; i<=genus; ++i)
    {
        s = Tools::convertToString(i);

        std::string a_i;
        a_i.append("a");
        a_i.append(s);
        std::string b_i;
        b_i.append("b");
        b_i.append(s);
        generatorsList.push_back(a_i);
        generatorsList.push_back(b_i);
    }

    for(int j=1; j<=numberOfPunctures; ++j)
    {
        s = Tools::convertToString(j);

        std::string c_j;
        c_j.append("c");
        c_j.append(s);
        generatorsList.push_back(c_j);

    }

    generators = generatorsList;

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
    for (k=2*genus; k<2*genus+numberOfPunctures; ++k)
    {
        l.first = k;
        l.second = 1;
        relation.push_back(l);
        (cusps[k-2*genus]).clear();
        (cusps[k-2*genus]).push_back(l);
    }

    relations = {relation};

    closedSurfaceGroup = (numberOfPunctures == 0);
}

std::vector<Word> DiscreteGroup::getWordsOfLengthLessThan(int n) const
{
    std::vector<Word> Words, WordsLengthj;

    for (int j=0; j<n ; ++j)
    {
        WordsLengthj = getWordsOfLengthEqualTo(j);
        Words.insert(Words.end(), WordsLengthj.begin(), WordsLengthj.end());
    }
    return Words;
}

std::vector<Word> DiscreteGroup::getWordsOfLengthEqualTo(int n) const
{
    std::vector<Word> output;
    letter l;
    Word w, wNew;
    int numberOfGenerators = generators.size();
    if (n <= 0)
    {
        std::cout << "There are no words of negative length" << std::endl;
    }
    if (n==1)
    {
        w.push_back(l);
        for (int i=0; i<numberOfGenerators; ++i)
        {
            l.first = i;
            l.second = 1;
            w.back() = l;
            output.push_back(w);
            l.second = -1;
            w.back() = l;
            output.push_back(w);
        }
        return output;
    }
    w.reserve(n-1);
    wNew.reserve(n);
    std::vector<Word> outPrevious = getWordsOfLengthEqualTo(n-1);
    int previousSize = outPrevious.size();
    for (int j=0; j<previousSize; ++j)
    {
        w = outPrevious[j];
        wNew = w;
        wNew.push_back(l);
        int lastGen = w.back().first;
        for (int k=0; k< numberOfGenerators; ++k)
        {
            if (k != lastGen)
            {
                l.first = k;
                l.second = 1;
                wNew.back() = l;
                output.push_back(wNew);
                l.second = -1;
                wNew.back() = l;
                output.push_back(wNew);
            } else
            {
                wNew.back() = w.back();
                output.push_back(wNew);
            }
        }
    }
    return output;
}

std::vector<Word> DiscreteGroup::getWordsOfNonRepeatingLettersLengthEqualTo(int n) const
{

    std::vector<Word> output;
    letter l;
    Word w,wNew;
    int numberOfGenerators = generators.size();
    if (n <= 0)
    {
        std::cout << "There are no Words of negative length" << std::endl;
    }
    if (n==1)
    {
        w.push_back(l);
        for (int i=0; i<numberOfGenerators; ++i)
        {
            l.first = i;
            l.second = 1;
            w.back() = l;
            output.push_back(w);
            l.second = -1;
            w.back() = l;
            output.push_back(w);
        }
        return output;
    }
    w.reserve(n-1);
    wNew.reserve(n);
    std::vector<Word> outPrevious = getWordsOfLengthEqualTo(n-1);
    int previousSize = outPrevious.size();
    for (int j=0; j<previousSize; ++j)
    {
        w = outPrevious[j];
        wNew = w;
        wNew.push_back(l);
        int lastGen = w.back().first;
        for (int k=0; k< numberOfGenerators; ++k)
        {
            if (k != lastGen)
            {
                l.first = k;
                l.second = 1;
                wNew.back() = l;
                output.push_back(wNew);
                l.second = -1;
                wNew.back() = l;
                output.push_back(wNew);
            }
        }
    }
    return output;
}

std::vector<Word> DiscreteGroup::getWordsOfNonRepeatingLettersLengthLessThan(int n) const
{
    std::vector<Word> Words,WordsStore;

    for (int j=1; j<n ; ++j)
    {
        WordsStore = getWordsOfNonRepeatingLettersLengthEqualTo(j);
        Words.insert(Words.end(), WordsStore.begin(), WordsStore.end());
    }
    return Words;
}

std::vector<generatorName> DiscreteGroup::getGenerators() const
{
    return generators;
}

bool DiscreteGroup::isClosedSurfaceGroup() const
{
    return closedSurfaceGroup;
}

std::vector<Word> DiscreteGroup::getRelations() const
{
    return relations;
}

std::vector<Word> DiscreteGroup::getCusps() const
{
    return cusps;
}

std::vector<Word> DiscreteGroup::getPairingsClosedSurfaceFromVertex() const
{
    assert(isClosedSurfaceGroup());

    std::vector<Word> res;
    int genus = generators.size()/2;
    res.reserve(4*genus);
    Word store;
    res.push_back(store);
    for(int j=0; j!=genus; ++j)
    {
        store=store*Word({letter(2*j,1)});
        res.push_back(store);
        store=store*Word({letter(2*j+1,1)});
        res.push_back(store);
        store=store*Word({letter(2*j,-1)});
        res.push_back(store);
        store=store*Word({letter(2*j+1,-1)});
        res.push_back(store);
    }
    res.pop_back();
    return res;
}

std::vector<Word> DiscreteGroup::getPairingsClosedSurfaceToVertex() const
{
    assert(isClosedSurfaceGroup());
    std::vector<Word> output;
    int genus = generators.size()/2;
    output.reserve(4*genus);

    for (const auto & w : getPairingsClosedSurfaceFromVertex())
    {
        output.push_back(w.inverse());
    }
    return output;
}

std::vector<Word> DiscreteGroup::getSidePairingsClosedSurface() const
{
    assert(isClosedSurfaceGroup());

    std::vector<Word> res;
    int genus = generators.size()/2;
    res.resize(4*genus);
    Word store, extra;
    for (int i=0; i!=genus; ++i)
    {
        extra = store*Word({letter(2*i,1),letter(2*i+1,1),letter(2*i,-1)})*store.inverse();
        res[4*i] = extra;
        res[4*i + 2] = extra.inverse();
        extra = store*Word({letter(2*i,1),letter(2*i+1,1),
                            letter(2*i,-1), letter(2*i+1,-1), letter(2*i,-1)})*store.inverse();
        res[4*i + 1] = extra;
        res[4*i + 3] = extra.inverse();
        store = store*Word({letter(2*i,1), letter(2*i+1,1), letter(2*i,-1), letter(2*i+1,-1)});
    }
    return res;
}

std::vector<Word> DiscreteGroup::getPairingsClosedSurfaceAroundVertices() const
{
    assert(isClosedSurfaceGroup());

    int genus = generators.size()/2;
    std::vector<Word> output, toVertex, fromVertex, temp1, temp2;
    output.reserve(16*genus*genus-4*genus);
    fromVertex.reserve(4*genus);
    temp1.reserve(4*genus-1);
    temp2.reserve(4*genus);
    toVertex = getPairingsClosedSurfaceToVertex();
    fromVertex = getPairingsClosedSurfaceFromVertex();
    int j=0;
    for (const auto & w : fromVertex)
    {
        temp2 = toVertex;
        temp2.erase(temp2.begin()+j);
        if (((j%4) == 0) || ((j%4) == 1))
        {
            temp2.erase(temp2.begin() + (j==4*genus-3 ? 0 : j+2));
        }
        else
        {
            temp2.erase(temp2.begin()+j-1);
        }
        temp1 = w*temp2;
        output.insert(output.end(),temp1.begin(),temp1.end());
        ++j;
    }
    return output;
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
    for(unsigned int i=1; i<generators.size(); ++i)
    {
        out << ", " << generators[i];
    }
    out << " | ";
    std::vector<Word> relations = Gamma.relations;
    out << Gamma.getWordAsString(relations[0]);
    for(unsigned int i=1; i<relations.size(); ++i)
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


std::string DiscreteGroup::getWordAsString(const Word & w) const
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

    if (Tools::containsDuplicates(generators1) || Tools::containsDuplicates(generators2))
    {
        std::cout << "WARNING in DiscreteGroup::checkCompatibilityForAmalgamation: group generators contains duplicates!" << std::endl;
        return false;
    }
    if (Tools::haveCommonElements(generators1, generators2))
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

bool DiscreteGroup::checkCompatibilityforHNNextension(const DiscreteGroup &Gamma)
{
    std::vector<generatorName> generators = Gamma.getGenerators();

    if (Tools::containsDuplicates(generators))
    {
        std::cout << "WARNING in DiscreteGroup::checkCompatibilityforHNNextension: group generators contains duplicates!" << std::endl;
        return false;
    }
    if (Gamma.numberOfCusps() != 0)
    {
        std::cout << "WARNING in DiscreteGroup::checkCompatibilityforHNNextension: I don't handle cusps (for the moment)!" << std::endl;
        return false;
    }
    return true;
}

bool DiscreteGroup::findGeneratorIndex(int &outputIndex, const generatorName &a) const
{
    unsigned int i=0;
    for (const auto &generator : generators)
    {
        if (generator==a)
        {
            outputIndex = i;
            return true;
        }
        ++i;
    }
    return false;
}

DiscreteGroup DiscreteGroup::amalgamateOverInverse(const DiscreteGroup &Gamma1, const generatorName &a1,
                                                   const DiscreteGroup &Gamma2, const generatorName &a1inverse)
{
    int i1, j1;
    if (!Gamma1.findGeneratorIndex(i1, a1) || !Gamma2.findGeneratorIndex(j1, a1inverse))
    {
        std::cout << "WARNING in DiscreteGroup::amalgamateOverInverse: generator is not in the group!" << std::endl;
    }

    std::vector<generatorName> outputGenerators;
    std::vector<Word> outputRelations;

    if(checkCompatibilityForAmalgamation(Gamma1, Gamma2))
    {
        std::vector<generatorName> generators1 = Gamma1.getGenerators();
        std::vector<generatorName> generators2 = Gamma2.getGenerators();
        std::vector<Word> relations1 = Gamma1.getRelations();
        std::vector<Word> relations2 = Gamma2.getRelations();

        outputGenerators = generators1;
        outputGenerators.insert(outputGenerators.end(), generators2.begin(), generators2.end());

        Word w;
        outputRelations = relations1;

        for (auto w : relations2)
        {
            for (unsigned int j=0; j<w.size(); ++j)
            {
                w[j].first += generators1.size();
            }
            outputRelations.push_back(w);
        }

        w.clear();
        w.push_back(letter(i1,1));
        w.push_back(letter(j1 + generators1.size(),1));
        outputRelations.push_back(w);
    }

    return DiscreteGroup(outputGenerators, outputRelations);
}

DiscreteGroup DiscreteGroup::doHNNextensionOverInverse(const DiscreteGroup & Gamma, const generatorName &a, const generatorName &ainverse,
                                                       const generatorName & newGeneratorName)
{
    int i1, j1;
    if (!Gamma.findGeneratorIndex(i1, a) || !Gamma.findGeneratorIndex(j1, ainverse))
    {
        std::cout << "WARNING in DiscreteGroup::amalgamateOverInverse: generator is not in the group!" << std::endl;
    }

    std::vector<generatorName> outputGenerators;
    std::vector<Word> outputRelations;

    if (checkCompatibilityforHNNextension(Gamma))
    {
        outputGenerators = Gamma.getGenerators();
        outputGenerators.push_back(newGeneratorName);
        int N = outputGenerators.size();

        outputRelations = Gamma.getRelations();
        Word w;
        w.push_back(letter(j1, 1));
        w.push_back(letter(N-1, -1));
        w.push_back(letter(i1, 1));
        w.push_back(letter(N-1,1));
        outputRelations.push_back(w);
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
    Word w;
    w.push_back(letter(2, 1));
    w.push_back(letter(1, 1));
    w.push_back(letter(0, 1));
    relations.push_back(w);
    return;
}

void DiscreteGroup::rotateGenerators(int rightshift)
{
    int antishift = generators.size() - rightshift;
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

void DiscreteGroup::rotateWord(Word &w, int shift)
{
    int n = generators.size();
    for (unsigned int i=0; i<w.size(); i++)
    {
        w[i].first = (w[i].first + shift) % n;
    }
    return;
}
