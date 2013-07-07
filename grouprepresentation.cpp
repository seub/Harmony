#include "grouprepresentation.h"

template<typename T> GroupRepresentation<T>::GroupRepresentation()
{
}

template<typename T> GroupRepresentation<T>::GroupRepresentation(DiscreteGroup *Gamma) : Gamma(Gamma)
{
}

template<typename T> GroupRepresentation<T>::GroupRepresentation(DiscreteGroup *Gamma, std::vector<T> listOfMatrices) : Gamma(Gamma), listOfMatrices(listOfMatrices)
{
    if(listOfMatrices.size() != (Gamma -> getGenerators()).size())
    {
        std::cout << "WARNING : The list of matrices doesn't match the list of generators." << std::endl;
    }
}

template<typename T> bool GroupRepresentation<T>::checkRelations() const
{
    std::vector<word> relations = Gamma -> getRelations();
    T identity(complex(1.0,0.0),complex(0.0,0.0),complex(0.0,0.0),complex(1.0,0.0));
    for (unsigned int i=0;i<relations.size();i++)
    {

        if(!(evaluateRepresentation(relations[i]) == identity))
        {
            return false;
        }
    }
    return true;
}

template<typename T> T GroupRepresentation<T>::evaluateRepresentation(const word & w) const
{
    T store(1.0,0.0,0.0,0.0);
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

template<typename T> GroupRepresentation<T> GroupRepresentation<T>::conj(const T & A)
{
    std::vector<T> list(listOfMatrices.size());
    for (unsigned int i=0;i<=listOfMatrices.size();i++)
    {
        list[i] = A*listOfMatrices[i]*A.inverse();
    }
    GroupRepresentation<T> r(Gamma,list);
    return r;

}

template<typename T> GroupRepresentation<T> GroupRepresentation<T>::conjugate() const
{
    std::vector<T> list(listOfMatrices.size());
    for (unsigned int i=0;i<=listOfMatrices.size();i++)
    {
        list[i] = listOfMatrices[i].conjugate();
    }
    GroupRepresentation<T> r(Gamma,list);
    return r;
}

H2Polygon FuchsianRepresentation::generatePolygon(const H2Point &basePoint) const
{
    H2Polygon res;
    return res;

}
