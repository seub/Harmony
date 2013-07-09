#include "grouprepresentation.h"
#include "h2isometry.h"

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
            std::cout << "WARNING in GroupRepresentation<T>::checkRelations(): failed" << std::endl;
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

template <> H2Polygon IsomH2Representation::generatePolygon(const H2Point &basePoint) const
{
    H2Polygon res;
    if (Gamma->isClosedSurfaceGroup())
    {
        H2Point point = basePoint;
        res.addVertex(point);
        int genus = listOfMatrices.size()/2;
        for (int i=0; i<genus; i++)
        {
            point = listOfMatrices[2*i]*point;
            res.addVertex(point);
            point = listOfMatrices[2*i + 1]*point;
            res.addVertex(point);
            point = listOfMatrices[2*i].inverse()*point;
            res.addVertex(point);
            point = listOfMatrices[2*i + 1].inverse()*point;
            res.addVertex(point);
        }
        res.removeLastVertex();
    }
    else
    {
        std::cout << "WARNING in RealRepresentation::generatePolygon: not a closed surface group representation" << std::endl;
    }
    return res;
}

template <> void IsomH2Representation::setFenchelNielsenCoordinates(const std::vector<double> & lengthParameters,
                                                                  const std::vector<double> & twistParameters)
{
    return;
}




