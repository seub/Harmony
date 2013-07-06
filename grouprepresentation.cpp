#include "grouprepresentation.h"

GroupRepresentation::GroupRepresentation()
{
}

GroupRepresentation::GroupRepresentation(DiscreteGroup *Gamma) : Gamma(Gamma)
{
}

GroupRepresentation::GroupRepresentation(DiscreteGroup *Gamma, std::vector<SL2CMatrix> listOfMatrices) : Gamma(Gamma), listOfMatrices(listOfMatrices)
{
    if(listOfMatrices.size() != (Gamma -> getGenerators()).size())
    {
        std::cout << "WARNING : The list of matrices doesn't match the list of generators." << std::endl;
    }
}

bool GroupRepresentation::checkRelations() const
{
    std::vector<word> relations = Gamma -> getRelations();
    SL2CMatrix identity(complex(1.0,0.0),complex(0.0,0.0),complex(0.0,0.0),complex(1.0,0.0));
    for (unsigned int i=0;i<relations.size();i++)
    {

        if(!(evaluateRepresentation(relations[i]) == identity))
        {
            return false;
        }
    }
    return true;
}

SL2CMatrix GroupRepresentation::evaluateRepresentation(const word & w) const
{
    SL2CMatrix store(complex(1.0,0.0),complex(0.0,0.0),complex(0.0,0.0),complex(1.0,0.0));
    for (unsigned int j=0;j<w.size();j++)
    {
        if(w[j].second == 1)
        {
            store = store*listOfMatrices[w[j].first];
        }
        else if (w[j].second == -1)
        {
            store = store*listOfMatrices[w[j].first].inverse();
        }
        else
        {
            std::cout << "Hey, you fucked up, moron!" << std::endl;
        }
    }
    return store;
}

GroupRepresentation GroupRepresentation::conj(const SL2CMatrix & A)
{
    std::vector<SL2CMatrix> list(listOfMatrices.size());
    for (unsigned int i=0;i<=listOfMatrices.size();i++)
    {
        list[i] = A*listOfMatrices[i]*A.inverse();
    }
    GroupRepresentation r(Gamma,list);
    return r;

}

GroupRepresentation GroupRepresentation::conjugate() const
{
    std::vector<SL2CMatrix> list(listOfMatrices.size());
    for (unsigned int i=0;i<=listOfMatrices.size();i++)
    {
        list[i] = listOfMatrices[i].conjugate();
    }
    GroupRepresentation r(Gamma,list);
    return r;
}
