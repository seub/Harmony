#include "grouprepresentation.h"
#include "topologicalsurface.h"


template<typename T> GroupRepresentation<T>::GroupRepresentation(DiscreteGroup *Gamma) : Gamma(Gamma)
{
}


template<typename T> GroupRepresentation<T>::GroupRepresentation(DiscreteGroup *Gamma, std::vector<T> listOfMatrices) :
    Gamma(Gamma), generatorImages(listOfMatrices)
{
    if(listOfMatrices.size() != (Gamma -> getGenerators()).size())
    {
        std::cout << "WARNING : The list of matrices doesn't match the list of generators." << std::endl;
    }
}

template <typename T> DiscreteGroup GroupRepresentation<T>::getDiscreteGroup() const
{
    return *Gamma;
}

template<typename T> bool GroupRepresentation<T>::checkRelations() const
{
    bool res = true;
    std::vector<word> relations = Gamma -> getRelations();
    T identity;
    identity.setIdentity();
    for (unsigned int i=0;i<relations.size();i++)
    {
        T test = evaluateRepresentation(relations[i]);
        if(!(test == identity))
        {
            std::cout << "WARNING in GroupRepresentation<T>::checkRelations(): failed" << std::endl;
            std::cout << "In relation #" << i << "( " << Gamma->getWordAsString(relations[i]) << "), error: ";
            std::cout << test.error() << std::endl;
            res = false;
        }
    }
    return res;
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
    std::cout << "could not find generator name " << a << " in " << *this << std::endl;
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

template<typename T> std::vector<T> GroupRepresentation<T>::evaluateRepresentation(const std::vector<word> & listOfWords)
{
    std::vector<T> output;
    for(unsigned int i=0; i<listOfWords.size(); i++)
    {
        output.push_back(evaluateRepresentation(listOfWords[i]));
    }
    return output;
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

template <> H2Polygon IsomH2Representation::generateFundamentalDomainForNormalizedSurfaceGroup(const H2Point &basePoint) const
{
    H2Polygon res;
    if (Gamma->isClosedSurfaceGroup())
    {
        res.addVertex(basePoint);
        int genus = generatorImages.size()/2;
        H2Isometry f;
        f.setIdentity();

        for (int i = 0; i<genus; i++)
        {
            f = f*generatorImages[2*i];
            res.addVertex(f*basePoint);
            f = f*generatorImages[2*i+1];
            res.addVertex(f*basePoint);
            f = f*generatorImages[2*i].inverse();
            res.addVertex(f*basePoint);
            f = f*generatorImages[2*i+1].inverse();
            res.addVertex(f*basePoint);
        }
        res.removeLastVertex();
    }
    else
    {
        std::cout << "WARNING in IsomH2Representation::generateFundmentalDomainForNormalizedSurfaceGroup:"
                     " not a closed surface group representation" << std::endl;
    }
    return res;
}


template <> std::vector<H2Isometry> IsomH2Representation::getSidePairingsForNormalizedFundamentalDomain() const
{
    std::vector<H2Isometry> output;
    if (Gamma->isClosedSurfaceGroup())
    {
        int genus = generatorImages.size()/2;
        H2Isometry store,extra;
        store.setIdentity();

        for (int i = 0; i<genus; i++)
        {
            extra = store*generatorImages[2*i]*
                    generatorImages[2*i+1]*
                    generatorImages[2*i].inverse()*store.inverse();
            output.push_back(extra);
            output.push_back(extra.inverse());
            extra = store*generatorImages[2*i]*generatorImages[2*i+1]*
                    generatorImages[2*i].inverse()*
                    generatorImages[2*i+1].inverse()*generatorImages[2*i].inverse()*store.inverse();
            output.push_back(extra);
            output.push_back(extra.inverse());
            store = store*generatorImages[2*i]*generatorImages[2*i+1]*
                    generatorImages[2*i].inverse()*generatorImages[2*i+1].inverse();
        }
    }
    else
    {
        std::cout << "WARNING in RealRepresentation::generatePolygon: not a closed surface group representation" << std::endl;
    }
    return output;
}

template <> std::vector<H2Isometry> IsomH2Representation::getSidePairingsNormalizedToDepth(int n) const
{
    int genus = generatorImages.size()/2;
    std::vector<H2Isometry> output;
    output.reserve(Tools::exponentiation(4*genus,n));
    H2Isometry f;
    if (n<=0)
    {
        std::cout << "Error in IsomH2Representation::getSidePairingsNormalizedToDepth: Side pairings of 'negative'' depth" << std::endl;
        throw(0);
    }
    if (n==1)
    {
        output = getSidePairingsForNormalizedFundamentalDomain();
        return output;
    }
    std::vector<H2Isometry> previous = getSidePairingsNormalizedToDepth(n-1);
    output = previous;

    for (unsigned int j=0; j<previous.size(); j++)
    {
        f = previous[j];
        for (unsigned int k=0; k<previous.size(); k++)
        {
            output.push_back(f*previous[k]);
        }
    }
    return output;
}

template <> std::vector<H2Isometry> IsomH2Representation::getSidePairingsNormalizedAroundVertex() const
{
    int genus = generatorImages.size()/2;
    std::vector<H2Isometry> output;
    output.reserve(4*genus);
    H2Isometry f,store,ai,bi;
    store.setIdentity();
    for (int i=0; i<genus; i++)
    {
        ai = generatorImages[2*i];
        bi = generatorImages[2*i+1];
        store = store*ai;
        output.push_back(store.inverse());
        store = store*bi;
        output.push_back(store.inverse());
        store = store*ai.inverse();
        output.push_back(store.inverse());
        store = store*bi.inverse();
        output.push_back(store.inverse());
    }
    output.pop_back();
    return output;
}

template <> std::vector<H2Isometry> IsomH2Representation::getSidePairingsNormalizedAroundVertices() const
{
    int genus = generatorImages.size()/2;
    std::vector<H2Isometry> output,previous,storePrevious;
    output.reserve(16*genus*genus);
    H2Isometry store,ai,bi;
    store.setIdentity();
    previous = getSidePairingsNormalizedAroundVertex();
    previous.push_back(store);
    output = previous;

    for (int i=0; i<genus-1; i++)
    {
        ai = generatorImages[2*i];
        bi = generatorImages[2*i+1];

        store = store*ai;
        storePrevious = store*previous;
        output.insert(output.end(), storePrevious.begin(), storePrevious.end());

        store = store*bi;
        storePrevious = store*previous;
        output.insert(output.end(), storePrevious.begin(), storePrevious.end());

        store = store*ai.inverse();
        storePrevious = store*previous;
        output.insert(output.end(), storePrevious.begin(), storePrevious.end());

        store = store*bi.inverse();
        storePrevious = store*previous;
        output.insert(output.end(), storePrevious.begin(), storePrevious.end());

    }
    ai = generatorImages[2*genus-2];
    bi = generatorImages[2*genus-1];

    store = store*ai;
    storePrevious = store*previous;
    output.insert(output.end(), storePrevious.begin(), storePrevious.end());

    store = store*bi;
    storePrevious = store*previous;
    output.insert(output.end(), storePrevious.begin(), storePrevious.end());

    store = store*ai.inverse();
    storePrevious = store*previous;
    output.insert(output.end(), storePrevious.begin(), storePrevious.end());

    return output;
}

template <> void IsomH2Representation::setNormalizedPairOfPantsRepresentation(generatorName c1, generatorName c2, generatorName c3,
                                                                              double length1, double length2, double length3,
                                                                              generatorName normalized)
{
    if (length1 < 0 || length2 < 0 || length3 < 0)
    {
        std::cout << "ERROR in IsomH2Representation::setNormalizedPairOfPantsRepresentation: you gave me a negative length (seriously?)"
                  << std::endl;
    }
    if (normalized == c3)
    {
        length1 *= 0.5;
        length2 *= 0.5;
        length3 *= 0.5;
        Gamma->setPairOfPants(c1, c2, c3);
        double C1, C2, C3, S2, S3;
        C1 = cosh(length1);
        C2 = cosh(length2);
        C3 = cosh(length3);
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
        d =-C3 +S3;
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
        std::cout << "ERROR in IsomH2Representation::setNormalizedPairOfPantsRepresentation: ";
        std::cout << "Normalizer has to be one of the generators!" << std::endl;
        return;
    }
}



template <> bool IsomH2Representation::checkCompatibilityOfFNcoordinates(const std::vector<double> & lengths,
                                                                         const std::vector<double> & twists)
{
    int N = lengths.size();
    if ((int) twists.size() != N)
    {
        std::cout << "ERROR in IsomH2Representation::setFenchelNielsenCoordinatesUnnormalized: ";
        std::cout << "number of lengths and twists don't match!" << std::endl;
        return false;
    }
    else if (N%3 != 0)
    {
        std::cout << "ERROR in IsomH2Representation::setFenchelNielsenCoordinatesUnnormalized: ";
        std::cout << "number of lengths and twists has to be a multiple of 3!" << std::endl;
        return false;
    }
    else if (N<3)
    {
        std::cout << "ERROR in IsomH2Representation::setFenchelNielsenCoordinatesUnnormalized: ";
        std::cout << "at least 3 lengths and twists are needed!" << std::endl;
        return false;
    }
    return true;
}

template <> std::vector<IsomH2Representation> IsomH2Representation::getPantsRepresentations(const std::vector<double> & lengths,
                                                                                            const std::vector<double> & twists,
                                                                                            const std::vector<DiscreteGroup *> &outputDiscreteGroups)
{
    int N = lengths.size();
    std::vector<IsomH2Representation> rhos;
    if (IsomH2Representation::checkCompatibilityOfFNcoordinates(lengths, twists))
    {
        int g = N/3 + 1;
        TopologicalSurface S(g, 0);
        std::vector<DiscreteGroup> V = S.getPantsDecomposition();

        for (unsigned int k=0; k<V.size(); k++)
        {
            rhos.push_back(IsomH2Representation(outputDiscreteGroups[k]));
        }

        std::vector<generatorName> generators = V[0].getGenerators();
        rhos[0].setNormalizedPairOfPantsRepresentation(generators[0], generators[1], generators[2],
                lengths[0], lengths[1], lengths[0],
                generators[1]);

        for (int j = 0; j<g-2; j++)
        {
            generators = V[2*j+1].getGenerators();
            rhos[2*j+1].setNormalizedPairOfPantsRepresentation(generators[0], generators[1], generators[2],
                    lengths[3*j+1], lengths[3*j+2], lengths[3*j+3],
                    generators[0]);
            generators = V[2*j+2].getGenerators();
            rhos[2*j+2].setNormalizedPairOfPantsRepresentation(generators[0], generators[1], generators[2],
                    lengths[3*j+4], lengths[3*j+3], lengths[3*j+2],
                    generators[0]);
        }

        generators = V[2*g-3].getGenerators();
        rhos[2*g-3].setNormalizedPairOfPantsRepresentation(generators[0], generators[1], generators[2],
                lengths[3*g-5], lengths[3*g-4], lengths[3*g-4],
                generators[0]);


    }
    return rhos;
}

template <> IsomH2Representation IsomH2Representation::amalgamateOverInverse(DiscreteGroup *outputDiscreteGroup,
                                                                             const IsomH2Representation & rho1, const generatorName &a1,
                                                                             const IsomH2Representation & rho2,
                                                                             const generatorName &a1inverse)
{
    *outputDiscreteGroup = DiscreteGroup::amalgamateOverInverse(*(rho1.Gamma), a1, *(rho2.Gamma), a1inverse);
    std::vector<H2Isometry> generatorImages1 = rho1.getGeneratorImages();
    std::vector<H2Isometry> generatorImages2 = rho2.getGeneratorImages();
    std::vector<H2Isometry> outputGeneratorImages = generatorImages1;

    outputGeneratorImages.insert(outputGeneratorImages.end(), generatorImages2.begin(), generatorImages2.end());

    return IsomH2Representation(outputDiscreteGroup, outputGeneratorImages);
}


template <> IsomH2Representation IsomH2Representation::doHNNextensionOverInverse(DiscreteGroup *outputDiscreteGroup, const IsomH2Representation &rho,
                                                                                 const generatorName &a,
                                                                                 const generatorName &ainverse,
                                                                                 const generatorName &newGeneratorName,
                                                                                 const H2Isometry & conjugator)
{
    *outputDiscreteGroup = DiscreteGroup::doHNNextensionOverInverse(*(rho.Gamma), a, ainverse, newGeneratorName);
    std::vector<H2Isometry> outputGeneratorImages = rho.getGeneratorImages();
    outputGeneratorImages.push_back(conjugator);

    return IsomH2Representation(outputDiscreteGroup, outputGeneratorImages);
}




template class GroupRepresentation<H2Isometry>;
template class GroupRepresentation<H3Isometry>;
template class GroupRepresentation<SL2RMatrix>;
template class GroupRepresentation<SL2CMatrix>;
