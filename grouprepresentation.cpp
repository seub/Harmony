#include "grouprepresentation.h"


template<typename T> GroupRepresentation<T>::GroupRepresentation(DiscreteGroup *Gamma) : Gamma(Gamma)
{
}


template<typename T> GroupRepresentation<T>::GroupRepresentation(DiscreteGroup *Gamma, std::vector<T> listOfMatrices) : Gamma(Gamma), generatorImages(listOfMatrices)
{
    if(listOfMatrices.size() != (Gamma -> getGenerators()).size())
    {
        std::cout << "WARNING : The list of matrices doesn't match the list of generators." << std::endl;
    }
}

template<typename T> bool GroupRepresentation<T>::checkRelations() const
{
    std::vector<word> relations = Gamma -> getRelations();
    T identity;
    identity.setIdentity();
    for (unsigned int i=0;i<relations.size();i++)
    {
        T test = evaluateRepresentation(relations[i]);
        if(!(test == identity))
        {
            std::cout << "WARNING in GroupRepresentation<T>::checkRelations(): failed" << std::endl;
            std::cout << test << " should be identity" << std::endl;
            return false;
        }
    }
    return true;
}

template <typename T> std::vector<T> GroupRepresentation<T>::getGeneratorImages() const
{
    return generatorImages;
}

template <typename T> bool GroupRepresentation<T>::getGeneratorImage(const generatorName &a, T & output) const
{
    std::vector<generatorName> generators = Gamma->getGenerators();
    for (unsigned int i=0; i<generators.size(); i++)
    {
        if (generators[i] == a)
        {
            output = generatorImages[i];
            return true;
        }
    }
    return false;
}

template<typename T> T GroupRepresentation<T>::evaluateRepresentation(const word & w) const
{
    T store;
    store.setIdentity();
    for (unsigned int j=0;j<w.size();j++)
    {
        if(w[j].second == 1)
        {
            store = store*generatorImages[w[j].first];
        }
        else if (w[j].second == -1)
        {
            store = store*generatorImages[w[j].first].inverse();
        }
    }
    return store;
}

template<typename T> GroupRepresentation<T> GroupRepresentation<T>::conj(const T & A) const
{
    std::vector<T> list;
    for (unsigned int i=0;i<generatorImages.size();i++)
    {
        list.push_back(A*generatorImages[i]*A.inverse());
    }
    GroupRepresentation<T> r(Gamma,list);
    return r;

}



template<typename T> void GroupRepresentation<T>::rotateGenerators(int shift)
{
    Gamma->rotateGenerators(shift);
    int antishift = generatorImages.size() - shift;
    rotate(generatorImages.begin(), generatorImages.begin()+ antishift, generatorImages.end());

    return;
}

template<> GroupRepresentation<SL2CMatrix> GroupRepresentation<SL2CMatrix>::conjugate() const
{
    std::vector<SL2CMatrix> list(generatorImages.size());
    for (unsigned int i=0;i<=generatorImages.size();i++)
    {
        list[i] = generatorImages[i].conjugate();
    }
    GroupRepresentation<SL2CMatrix> r(Gamma,list);
    return r;
}

template <> H2Polygon IsomH2Representation::generatePolygon(const H2Point &basePoint) const
{
    H2Polygon res;
    if (Gamma->isClosedSurfaceGroup())
    {
        H2Point point = basePoint;
        res.addVertex(point);
        int genus = generatorImages.size()/2;
        for (int i=0; i<genus; i++)
        {
            point = generatorImages[2*i]*point;
            res.addVertex(point);
            point = generatorImages[2*i + 1]*point;
            res.addVertex(point);
            point = generatorImages[2*i].inverse()*point;
            res.addVertex(point);
            point = generatorImages[2*i + 1].inverse()*point;
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


template <> void IsomH2Representation:: setNormalizedPairOfPantsRepresentation(generatorName c1, generatorName c2, generatorName c3,
                                                                               double length1, double length2, double length3, generatorName normalized)
{
    if (length1 < 0 || length2 < 0 || length3 < 0)
    {
        std::cout << "ERROR in IsomH2Representation::setNormalizedPairOfPantsRepresentation: you gave me a negative length (seriously?)" << std::endl;
    }
    if (normalized == c3)
    {
        length1 *= 0.5;
        length2 *= 0.5;
        length3 *= 0.5;
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

        generatorImages.clear();
        H2Isometry f;
        f.setSL2Rmatrix(M1);
        generatorImages.push_back(f);
        f.setSL2Rmatrix(M2);
        generatorImages.push_back(f);
        f.setSL2Rmatrix(M3);
        generatorImages.push_back(f);
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


template <> IsomH2Representation IsomH2Representation::amalgamateOverInverse(DiscreteGroup *outputDiscreteGroup,
                                                                             const IsomH2Representation & rho1, const generatorName &a1,
                                                                             const generatorName &a1left,
                                                                             const IsomH2Representation & rho2, const generatorName &a1inverse,
                                                                             const generatorName &a1inverseleft, const generatorName &newGeneratorName,
                                                                             double twist)
{
    *outputDiscreteGroup = DiscreteGroup::amalgamateOverInverse(*(rho1.Gamma), a1, *(rho2.Gamma), a1inverse, newGeneratorName);
    H2Isometry f1, f1left, f2, f2left;
    if (!(rho1.getGeneratorImage(a1, f1) && rho1.getGeneratorImage(a1left, f1left) &&
          rho2.getGeneratorImage(a1inverse, f2) && rho2.getGeneratorImage(a1inverseleft, f2left)))
    {
        std::cout << "ERROR in IsomH2Representation::amalgamateOverInverse: generator name is not in the group!" << std::endl;
    }


    H2Isometry conjugator = H2Isometry::findConjugatorForGluing(f1, f1left, f2, f2left, twist);
    IsomH2Representation rho2new(rho2.Gamma);
    rho2new = rho2.conj(conjugator);

    std::vector<H2Isometry> generatorImages1 = rho1.getGeneratorImages();
    std::vector<H2Isometry> generatorImages2 = rho2new.getGeneratorImages();
    std::vector<H2Isometry> outputGeneratorImages = generatorImages1;
    std::vector<generatorName> generators2 = rho2.Gamma->getGenerators();

    for (unsigned int i = 0; i<generators2.size(); i++)
    {
        if (generators2[i] != a1inverse)
        {
            outputGeneratorImages.push_back(generatorImages2[i]);
        }
    }

    return IsomH2Representation(outputDiscreteGroup, outputGeneratorImages);
}



template class GroupRepresentation<H2Isometry>;
template class GroupRepresentation<H3Isometry>;
template class GroupRepresentation<SL2RMatrix>;
template class GroupRepresentation<SL2CMatrix>;
