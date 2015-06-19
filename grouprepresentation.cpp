#include "grouprepresentation.h"
#include "topologicalsurface.h"

template<typename T> GroupRepresentation<T>::GroupRepresentation() : Gamma(0)
{
}

template<typename T> GroupRepresentation<T>::GroupRepresentation(DiscreteGroup *Gamma) : Gamma(Gamma)
{
}


template<typename T> GroupRepresentation<T>::GroupRepresentation(DiscreteGroup *Gamma, std::vector<T> listOfMatrices) :
    Gamma(Gamma), generatorImages(listOfMatrices)
{
    if(listOfMatrices.size() != (Gamma -> getGenerators()).size())
    {
        throw(QString("Error in GroupRepresentation<T>::GroupRepresentation : The list of matrices doesn't match the list of generators"));
    }
}

template <typename T> DiscreteGroup GroupRepresentation<T>::getDiscreteGroup() const
{
    return *Gamma;
}

template<typename T> bool GroupRepresentation<T>::checkRelations() const
{
    bool res = true;
    std::vector<Word> relations = Gamma -> getRelations();
    T identity;
    identity.setIdentity();

    unsigned int i = 0;
    for (const auto &r : relations)
    {
        T test = evaluateRepresentation(r);
        if(!(test == identity))
        {
            std::cout << "WARNING in GroupRepresentation<T>::checkRelations(): failed" << std::endl;
            std::cout << "In relation #" << i << "( " << Gamma->getWordAsString(r) << "), error: ";
            std::cout << test.error() << std::endl;
            res = false;
        }
        ++i;
    }
    return res;
}

template <typename T> std::vector<T> GroupRepresentation<T>::getGeneratorImages() const
{
    return generatorImages;
}

template <typename T> bool GroupRepresentation<T>::getGeneratorImage(const generatorName &a, T &output) const
{
    std::vector<generatorName> generators = Gamma->getGenerators();
    unsigned int i=0;
    for (const auto &g : generators)
    {
        if (g == a)
        {
            output = generatorImages[i];
            return true;
        }
        ++i;
    }

    std::stringstream errorMessage;
    errorMessage << "Error in GroupRepresentation<T>::getGeneratorImage: could not find generator name " << a << " in " << *this << std::endl;
    throw(QString::fromStdString(errorMessage.str()));
    return false;
}

template<typename T> T GroupRepresentation<T>::evaluateRepresentation(const Word & w) const
{
    T res = 1;
    Word wc  = Word::contract(w);
    std::vector<letter> letters = wc.getLetters();
    for (const auto &l : letters)
    {
        if(l.second >= 0)
        {
            res = res*Tools::exponentiation(generatorImages[l.first], l.second);
        }
        else
        {
            res = res*Tools::exponentiation(generatorImages[l.first], -l.second).inverse();
        }
    }
    return res;
}

template<typename T> std::vector<T> GroupRepresentation<T>::evaluateRepresentation(const std::vector<Word> & listOfWords) const
{
    std::vector<T> output;
    output.reserve(listOfWords.size());
    for(const auto & word : listOfWords)
    {
        output.push_back(evaluateRepresentation(word));
    }
    return output;
}

template<typename T> GroupRepresentation<T> GroupRepresentation<T>::conjugate(const T &conjugator) const
{
    std::vector<T> list;
    list.reserve(generatorImages.size());
    for (const auto &genim : generatorImages)
    {
        list.push_back(conjugator*genim*conjugator.inverse());
    }
    return GroupRepresentation<T>(Gamma,list);

}



template<typename T> void GroupRepresentation<T>::rotateGenerators(int shift)
{
    Gamma->rotateGenerators(shift);
    int antishift = generatorImages.size() - shift;
    rotate(generatorImages.begin(), generatorImages.begin()+ antishift, generatorImages.end());

    return;
}

template<> GroupRepresentation<SL2CMatrix> GroupRepresentation<SL2CMatrix>::bar() const
{
    std::vector<SL2CMatrix> list(generatorImages.size());
    for (unsigned int i=0;i<=generatorImages.size();i++)
    {
        list[i] = generatorImages[i].bar();
    }
    GroupRepresentation<SL2CMatrix> r(Gamma,list);
    return r;
}

template<> H2Polygon IsomH2Representation::generatePolygon(const H2Point &basePoint) const
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
        throw(QString("Error in IsomH2Representation::generatePolygon(const H2Point &basePoint): not a closed surface group representation"));
    }

    if (!res.isConvex())
    {
        std::cout << "WARNING in IsomH2Representation::generatePolygon(const H2Point &basePoint):"
                  << " polygon is not convex" << std::endl;
    }

    return res;
}

template <> void IsomH2Representation::generatePolygon(const H2Point &basePoint, H2Polygon &polygon) const
{
    std::vector<H2Point> newVertices(polygon.nbVertices());

    newVertices[0] = basePoint;
    int genus = generatorImages.size()/2;
    H2Isometry f = H2Isometry::identity();

    int i, k=0;
    for (i = 0; i+1<genus; ++i)
    {
        f = f*generatorImages[2*i];
        newVertices[++k] = f*basePoint;
        f = f*generatorImages[2*i+1];
        newVertices[++k] = f*basePoint;
        f = f*generatorImages[2*i].inverse();
        newVertices[++k] = f*basePoint;
        f = f*generatorImages[2*i+1].inverse();
        newVertices[++k] = f*basePoint;
    }

    f = f*generatorImages[2*i];
    newVertices[++k] = f*basePoint;
    f = f*generatorImages[2*i+1];
    newVertices[++k] = f*basePoint;
    f = f*generatorImages[2*i].inverse();
    newVertices[++k] = f*basePoint;

    polygon.setVertices(newVertices);

    return;
}

template <> H2Polygon IsomH2Representation::generatePolygon(int tilingSize) const
{
    clock_t start, end;
    start = clock();

    double step = 1.0/tilingSize;

    H2Point p;
    H2Polygon polygon, bestPolygon;
    polygon.vertices.resize(2*generatorImages.size());
    double currentNorm, bestNorm = 1.0/ERROR;
    Complex z;

    double x = -1.0, y = -1.0;
    for(int i=1; i<2*tilingSize; i++)
    {
        x += step;
        y = -1.0;
        for (int j=1; j<2*tilingSize; j++)
        {
            y += step;
            z = Complex(x,y);
            if (norm(z) < 1.0)
            {
                p.setDiskCoordinate(Complex(x, y));
                generatePolygon(p, polygon);
                if (polygon.isConvex())
                {
                    currentNorm = polygon.norm4();
                    if (currentNorm < bestNorm)
                    {
                        bestNorm = currentNorm;
                        bestPolygon = polygon;
                    }
                }
            }
        }
    }

    end = clock();
    qDebug() << (end - start)*1.0/CLOCKS_PER_SEC << " time spend in (stupid) IsomH2Representation::generatePolygon(int tilingSize)";
    return bestPolygon;
}

/*
template <> std::vector<Word> IsomH2Representation::getWordSidePairings() const
{
    assert(Gamma->isClosedSurfaceGroup());

    std::vector<Word> res;
    int genus = Gamma->getGenerators().size()/2;
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
}*/

template <> DiscreteGroup* IsomH2Representation::getGroup() const
{
    return Gamma;
}

/*
template <> std::vector<Word> IsomH2Representation::getVertexPairings() const
{
    assert(Gamma->isClosedSurfaceGroup());

    std::vector<Word> res;
    int genus = Gamma->getGenerators().size()/2;
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
}*/

template <> std::vector<H2Isometry> IsomH2Representation::getSidePairingsForNormalizedFundamentalDomain() const
{
    return evaluateRepresentation(getGroup()->getSidePairingsClosedSurface());
}

template <> std::vector<H2Isometry> IsomH2Representation::getSidePairingsNormalizedToDepth(int n) const
{
    int genus = generatorImages.size()/2;
    std::vector<H2Isometry> output;
    output.reserve(Tools::exponentiation(4*genus,n));
    if (n<=0)
    {
        throw(QString("Error in IsomH2Representation::getSidePairingsNormalizedToDepth: Side pairings of 'negative'' depth"));
    }
    if (n==1)
    {
        output = getSidePairingsForNormalizedFundamentalDomain();
        return output;
    }
    std::vector<H2Isometry> previous = getSidePairingsNormalizedToDepth(n-1);
    output = previous;

    for (const auto &f : previous)
    {
        for (const auto &g : previous)
        {
            output.push_back(f*g);
        }
    }
    return output;
}

template <> std::vector<H2Isometry> IsomH2Representation::getSidePairingsNormalizedAroundVertex() const
{
    int genus = generatorImages.size()/2;
    std::vector<H2Isometry> output;
    output.reserve(4*genus-1);
    std::vector<Word> temp = getGroup()->getPairingsClosedSurfaceToVertex();
    temp.erase(temp.begin());
    for (const auto & w : temp)
    {
        output.push_back(evaluateRepresentation(w));
    }
    return output;
}

template <> std::vector<H2Isometry> IsomH2Representation::getSidePairingsNormalizedAroundVertices() const
{
    int genus = generatorImages.size()/2;
    std::vector<H2Isometry> output;
    output.reserve(16*genus*genus-4*genus);
    std::vector<Word> temp = getGroup()->getPairingsClosedSurfaceAroundVertices();

    for (const auto & w : temp)
    {
        output.push_back(evaluateRepresentation(w));
    }
    return output;
}

template <> std::vector<H2Isometry> IsomH2Representation::getSidePairingsNormalizedAroundVerticesToDepth(int n)
{
    int genus = generatorImages.size()/2;
    std::vector<H2Isometry> output,aroundVertices,shortWords,store;
    aroundVertices = getSidePairingsNormalizedAroundVertices();
    store.reserve(aroundVertices.size());
    output.reserve(Tools::exponentiation(4*genus,n));
    output = aroundVertices;
    std::vector<Word> Words = (Gamma->getWordsOfNonRepeatingLettersLengthLessThan(n));
    shortWords = evaluateRepresentation(Words);
    for (unsigned int i=0; i<shortWords.size(); i++)
    {
        store = shortWords[i]*aroundVertices;
        output.insert(output.end(), store.begin(),store.end());
    }
    return output;
}

template <> void IsomH2Representation::setNormalizedPairOfPantsRepresentation(generatorName c1, generatorName c2, generatorName c3,
                                                                              double length1, double length2, double length3,
                                                                              generatorName normalized)
{
    if (length1 < 0 || length2 < 0 || length3 < 0)
    {
        throw(QString("ERROR in IsomH2Representation::setNormalizedPairOfPantsRepresentation: you gave me a negative length (seriously?)"));
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
        H2Isometry f1(SL2RMatrix(a, b, c, d));

        a = (-C2*S3 + C2*C3 + C1)/S3;
        b = (-C1 - C2*C3 + S2*S3)/S3;
        c = (C1 + C2*C3 + S2*S3)/S3;
        d = (-C2*S3 -C2*C3 - C1)/S3;
        H2Isometry f2(SL2RMatrix(a, b, c, d));

        a = -C3 -S3;
        b = 0;
        c = 0;
        d =-C3 +S3;
        H2Isometry f3(SL2RMatrix(a, b, c, d));

        generatorImages = {f1, f2, f3};
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
        throw(QString("ERROR in IsomH2Representation::setNormalizedPairOfPantsRepresentation: Normalizer has to be one of the generators!"));
    }
}


template <> void IsomH2Representation::setNormalizedPairOfPantsRepresentation(generatorName c1, generatorName c2, generatorName c3,
                                                                              double C1, double C2, double C3,
                                                                              double S1, double S2, double S3,
                                                                              generatorName normalized)
{
    if (S1<0 || S2 < 0 || S3 < 0)
    {
        throw(QString("ERROR in IsomH2Representation::setNormalizedPairOfPantsRepresentation: you gave me a negative length (seriously?)"));
    }
    if (normalized == c3)
    {
        Gamma->setPairOfPants(c1, c2, c3);

        generatorImages.clear();
        H2Isometry f;

        Complex W, Z, A, U;

        Z = Complex(-C1*S3 , -C3*C1 - C2*C3*C3 + S2*S3*S3);
        W = Complex(-C3*S2*S3 + S3*C1 + S3*C2*C3 , -C1*C3 - C2);
        U = Z/conj(Z);
        A = W/Z;

        f.setDiskCoordinates(U, A);
        generatorImages.push_back(f);

        Z = Complex(-C2*S3 , -C1-C2*C3);
        W = Complex(-S2*S3 , -C2*C3-C1);
        U = Z/conj(Z);
        A = W/Z;

        f.setDiskCoordinates(U, A);
        generatorImages.push_back(f);

        U = 1.0;
        A = Complex(0.0, -S3/C3);

        f.setDiskCoordinates(U, A);
        generatorImages.push_back(f);
        return;
    }
    else if (normalized == c1)
    {
        setNormalizedPairOfPantsRepresentation(c2, c3, c1, C2, C3, C1, S2, S3, S1, c1);
        rotateGenerators(1);
        return;
    }
    else if (normalized == c2)
    {
        setNormalizedPairOfPantsRepresentation(c3, c1, c2, C3, C1, C2, S3, S1, S2, c2);
        rotateGenerators(2);
        return;
    }
    else
    {
        throw(QString("ERROR in IsomH2Representation::setNormalizedPairOfPantsRepresentation: Normalizer has to be one of the generators!"));
    }
}

template <> void IsomH2Representation::setNiceRepresentation()
{
    *Gamma = DiscreteGroup(TopologicalSurface(2, 0));

    H2Isometry A1, B1, A2, B2, a1, b1, a2, b2;

    H2Point p1, p2;
    H2Geodesic L1, L2;

    Complex I(0.0, 1.0);

    Complex z1 = 1.0/sqrt(sqrt(2.0));
    Complex u = Complex(1.0,1.0)/sqrt(2.0);

    p1.setDiskCoordinate(z1);
    p2.setDiskCoordinate(u*z1);
    L1.setPassingThroughTwoPoints(p1, p2);
    p1.setDiskCoordinate(I*u*z1);
    p2.setDiskCoordinate(I*z1);
    L2.setPassingThroughTwoPoints(p1, p2);

    A1.setByMappingGeodesic(L1, L2);

    p1.setDiskCoordinate(u*z1);
    p2.setDiskCoordinate(I*z1);
    L1.setPassingThroughTwoPoints(p1, p2);
    p1.setDiskCoordinate(-z1);
    p2.setDiskCoordinate(u*I*z1);
    L2.setPassingThroughTwoPoints(p1, p2);

    B1.setByMappingGeodesic(L1, L2);

    p1.setDiskCoordinate(-z1);
    p2.setDiskCoordinate(-u*z1);
    L1.setPassingThroughTwoPoints(p1, p2);
    p1.setDiskCoordinate(-I*u*z1);
    p2.setDiskCoordinate(-I*z1);
    L2.setPassingThroughTwoPoints(p1, p2);

    A2.setByMappingGeodesic(L1, L2);

    p1.setDiskCoordinate(-u*z1);
    p2.setDiskCoordinate(-I*z1);
    L1.setPassingThroughTwoPoints(p1, p2);
    p1.setDiskCoordinate(z1);
    p2.setDiskCoordinate(-I*u*z1);
    L2.setPassingThroughTwoPoints(p1, p2);

    B2.setByMappingGeodesic(L1, L2);


    a1 = A1.inverse()*B1.inverse()*A1;
    b1 = A1.inverse()*B1*A1*B1.inverse()*A1;
    b2 = A2*B1*A1.inverse()*B1.inverse()*A1;
    a2 = b2.inverse()*B2.inverse()*b2;



    p1.setDiskCoordinate(-z1);


    generatorImages = {a1, b1, a2, b2};
    return;
}



template <> bool IsomH2Representation::checkCompatibilityOfFNcoordinates(const std::vector<double> & lengths,
                                                                         const std::vector<double> & twists)
{
    int N = lengths.size();
    if ((int) twists.size() != N)
    {
        throw(QString("ERROR in IsomH2Representation::setFenchelNielsenCoordinatesUnnormalized: number of lengths and twists don't match!"));
        return false;
    }
    else if (N%3 != 0)
    {
        throw(QString("ERROR in IsomH2Representation::setFenchelNielsenCoordinatesUnnormalized: number of lengths and twists has to be a multiple of 3!"));
    }
    else if (N<3)
    {
        throw(QString("ERROR in IsomH2Representation::setFenchelNielsenCoordinatesUnnormalized: at least 3 lengths and twists are needed!"));
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

        for (unsigned int k=0; k<V.size(); ++k)
        {
            rhos.push_back(IsomH2Representation(outputDiscreteGroups[k]));
        }

        std::vector<generatorName> generators = V[0].getGenerators();
        rhos[0].setNormalizedPairOfPantsRepresentation(generators[0], generators[1], generators[2],
                lengths[0], lengths[1], lengths[0],
                generators[1]);

        for (int j = 0; j<g-2; ++j)
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
