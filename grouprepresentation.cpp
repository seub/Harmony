#include "grouprepresentation.h"
#include "h2isometry.h"

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

template<typename T> void GroupRepresentation<T>::rotateGenerators(int shift)
{
    Gamma->rotateGenerators(shift);
    rotate(listOfMatrices.begin(), listOfMatrices.begin()+shift, listOfMatrices.end());
    return;
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

template <> void IsomH2Representation::setNormalizedPairOfPantsRepresentation(generatorName c1, generatorName c2, generatorName c3,
                                                                              double length1, double length2, double length3, generatorName normalized)
{
    if (length1 < 0 || length2 < 0 || length3 < 0)
    {
        std::cout << "ERROR in IsomH2Representation::setNormalizedPairOfPantsRepresentation: you gave me a negative length (seriously?)" << std::endl;
    }
    if (normalized == c3)
    {
        Gamma->setPairOfPants(c1, c2, c3);
        double C1, C2, C3, S1, S2, S3;
        C1 = cosh(length1);
        C2 = cosh(length2);
        C3 = cosh(length3);
        S1 = sinh(length1);
        S2 = sinh(length2);
        S3 = sinh(length3);

        double a, b, c, d;
        a = (-C1*S3 + C1*C3 + C2)/S3;
        b = (-(C1 + C2*C3 - S2*S3)*(C3 + S3))/S3;
        c = ((C1 + C2*C3 + S2*S3)*(C3 - S3))/S3;
        d = (-C1*S3 -C1*C3 - C2)/S3;
        SL2RMatrix M1(a, b, c, d);

        a = (-C2*S3 + C2*C3 + C1)/S3;
        b = (-C1 - C2*C3 + S2*S3)/S3;
        c = (C1 + C2*C3 + S2*S3)/S3;
        d = (-C2*S3 -C2*C3 - C1)/S3;
        SL2RMatrix M2(a, b, c, d);

        a = -C3 -S3;
        b = 0;
        c = 0;
        d  =-C3 +S3;
        SL2RMatrix M3(a, b, c, d);

        listOfMatrices.clear();
        listOfMatrices.push_back(H2Isometry(M1));
        listOfMatrices.push_back(H2Isometry(M2));
        listOfMatrices.push_back(H2Isometry(M3));
        return;
    }
    else if (normalized == c1)
    {
        setNormalizedPairOfPantsRepresentation(c2, c3, c1, length2, length3, length1, c1);
        rotateGenerators(1);
        return;
    }
    else if (normalized == c2)
    {
        setNormalizedPairOfPantsRepresentation(c3, c1, c2, length3, length1, length2, c2);
        rotateGenerators(2);
        return;
    }
    else
    {
        std::cout << "ERROR in IsomH2Representation::setNormalizedPairOfPantsRepresentation: Normalizer has to be one of the generators!" << std::endl;
        return;
    }
}
