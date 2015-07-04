#include "grouprepresentation.h"
#include "topologicalsurface.h"
#include "fundamentaldomaingenerator.h"

template<typename T> GroupRepresentation<T>::GroupRepresentation()
{
}

template<typename T> GroupRepresentation<T>::GroupRepresentation(const DiscreteGroup &Gamma) : Gamma(Gamma)
{
}


template<typename T> GroupRepresentation<T>::GroupRepresentation(const DiscreteGroup &Gamma, std::vector<T> listOfMatrices) :
    Gamma(Gamma), generatorImages(listOfMatrices)
{
    if(listOfMatrices.size() != (Gamma.getGenerators()).size())
    {
        throw(QString("Error in GroupRepresentation<T>::GroupRepresentation : The list of matrices doesn't match the list of generators"));
    }
}

template <typename T> DiscreteGroup GroupRepresentation<T>::getDiscreteGroup() const
{
    return Gamma;
}

template<typename T> bool GroupRepresentation<T>::checkRelations() const
{
    bool res = true;
    std::vector<Word> relations = Gamma.getRelations();
    T identity;
    identity.setIdentity();

    uint i = 0;
    for (const auto &r : relations)
    {
        T test = evaluateRepresentation(r);
        if(!T::almostEqual(test, identity))
        {
            std::stringstream message;
            message << "WARNING in GroupRepresentation<T>::checkRelations(): failed";
            message << "In relation #" << i << "( " << Gamma.getWordAsString(r) << "), error: " << test.error();
            qDebug() << QString::fromStdString(message.str());
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
    std::vector<generatorName> generators = Gamma.getGenerators();
    uint i=0;
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
    T res = T(1);
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
    Gamma.rotateGenerators(shift);
    int antishift = generatorImages.size() - shift;
    rotate(generatorImages.begin(), generatorImages.begin()+ antishift, generatorImages.end());
}

template<> GroupRepresentation<SL2CMatrix> GroupRepresentation<SL2CMatrix>::bar() const
{
    std::vector<SL2CMatrix> list(generatorImages.size());
    for (uint i=0; i!=generatorImages.size(); ++i)
    {
        list[i] = generatorImages[i].bar();
    }
    GroupRepresentation<SL2CMatrix> r(Gamma,list);
    return r;
}

template <> DiscreteGroup GroupRepresentation<H2Isometry>::getDiscreteGroup() const
{
    return Gamma;
}

template <> std::vector<H2Isometry> GroupRepresentation<H2Isometry>::getSidePairings() const
{
    return evaluateRepresentation(Gamma.getSidePairings());
}

template <> std::vector<H2Isometry> GroupRepresentation<H2Isometry>::getPairingsFromVertex() const
{
<<<<<<< HEAD
    H2Point basePt, basePtEpsilonX, basePtEpsilonY, basePtEpsilonXMinus, basePtEpsilonYMinus;
    std::vector<H2Point> basePtImages, basePtImagesEpsilonX, basePtImagesEpsilonY, basePtImagesEpsilonXMinus, basePtImagesEpsilonYMinus;
    std::vector<H2Isometry> fromVertexPairings;
    fromVertexPairings = evaluateRepresentation(Gamma.getPairingsClosedSurfaceFromVertex());
    Complex previous;
    H2Polygon output;

    basePt.setDiskCoordinate(Complex(0.0,0.0));
    previous = basePt.getDiskCoordinate();
    basePtEpsilonX = basePt.pushPointHorizonalEpsilon(epsilon);
    basePtEpsilonY = basePt.pushPointVerticalEpsilon(epsilon);
    basePtEpsilonXMinus = basePt.pushPointHorizontalMinusEpsilon(epsilon);
    basePtEpsilonYMinus = basePt.pushPointVerticalMinusEpsilon(epsilon);

    for(const auto & f : fromVertexPairings)
    {
        basePtImagesEpsilonX.push_back(f*basePtEpsilonX);
        basePtImagesEpsilonY.push_back(f*basePtEpsilonY);
        basePtImagesEpsilonXMinus.push_back(f*basePtEpsilonXMinus);
        basePtImagesEpsilonYMinus.push_back(f*basePtEpsilonYMinus);
    }

    double dfx,dfy, previousNorm;
    dfx = (H2Point::diameter(basePtImagesEpsilonX) - H2Point::diameter(basePtImagesEpsilonXMinus))/(2*epsilon);
    dfy = (H2Point::diameter(basePtImagesEpsilonY) - H2Point::diameter(basePtImagesEpsilonYMinus))/(2*epsilon);
    previousNorm = dfx*dfx + dfy*dfy;

    while (previousNorm > .00000001)
    {
        basePt.setDiskCoordinate(previous - stepSize*Complex(dfx,dfy));
        basePtEpsilonX = basePt.pushPointHorizonalEpsilon(epsilon);
        basePtEpsilonY = basePt.pushPointVerticalEpsilon(epsilon);
        basePtEpsilonXMinus = basePt.pushPointHorizontalMinusEpsilon(epsilon);
        basePtEpsilonYMinus = basePt.pushPointVerticalMinusEpsilon(epsilon);

        basePtImagesEpsilonX.clear();
        basePtImagesEpsilonY.clear();
        basePtImagesEpsilonXMinus.clear();
        basePtImagesEpsilonYMinus.clear();

        for(const auto & f : fromVertexPairings)
        {
            basePtImagesEpsilonX.push_back(f*basePtEpsilonX);
            basePtImagesEpsilonY.push_back(f*basePtEpsilonY);
            basePtImagesEpsilonXMinus.push_back(f*basePtEpsilonXMinus);
            basePtImagesEpsilonYMinus.push_back(f*basePtEpsilonYMinus);
        }

        dfx = (H2Point::diameter(basePtImagesEpsilonX) - H2Point::diameter(basePtImagesEpsilonXMinus))/(2*epsilon);
        dfy = (H2Point::diameter(basePtImagesEpsilonY) - H2Point::diameter(basePtImagesEpsilonYMinus))/(2*epsilon);
/*
        if(previousNorm < dfx*dfx + dfy*dfy)
        {
            throw(QString("Error in generateFundamentalDomainOptimization: Suggestion of lack of convexity"));
        }
*/
        previousNorm = dfx*dfx + dfy*dfy;
        previous = basePt.getDiskCoordinate();
        if (norm(previous)>1)
        {
            throw(QString("Error in generateFundamentalDomainOptimization: basePt is outside the unit disk"));
        }
    }
    for(const auto & f : fromVertexPairings)
    {
        basePtImages.push_back(f*basePt);
    }

    /*std::cout << "basePt is an " << basePt << std::endl;
    std::cout << "Final diameter = " << H2Point::diameter(basePtImages) << std::endl;*/

    return H2Polygon(basePtImages);
=======
    return evaluateRepresentation(Gamma.getPairingsFromVertex());
>>>>>>> ddef4dec805edffad41304c6ad0d1c5ed086bde8
}

template <> std::vector<H2Isometry> GroupRepresentation<H2Isometry>::getPairingsAroundVertex() const
{
    return evaluateRepresentation(Gamma.getPairingsAroundVertex());
}

template <> std::vector<H2Isometry> GroupRepresentation<H2Isometry>::getPairingsAroundVertices() const
{
    return evaluateRepresentation(Gamma.getPairingsAroundVertices());
}


template <> H2Polygon GroupRepresentation<H2Isometry>::getFundamentalDomain(const H2Point &firstVertex) const
{
    std::vector<H2Isometry> pairingsFromVertex = getPairingsFromVertex();
    std::vector<H2Point> vertices;
    vertices.reserve(pairingsFromVertex.size());

    for (const auto &pairing : pairingsFromVertex)
    {
        vertices.push_back(pairing*firstVertex);
    }

    return H2Polygon(vertices);
}

template <> H2Polygon GroupRepresentation<H2Isometry>::getOptimalFundamentalDomain() const
{
    return FundamentalDomainGenerator(*this).getOptimalFundamentalDomain();
}




template <> void GroupRepresentation<H2Isometry>::setNormalizedPairOfPantsRepresentation(generatorName c1, generatorName c2, generatorName c3,
                                                                                         double C1, double C2, double C3,
                                                                                         double S1, double S2, double S3,
                                                                                         generatorName normalized)
{
    if (S1<0 || S2 < 0 || S3 < 0)
    {
        throw(QString("ERROR in GroupRepresentation<H2Isometry>::setNormalizedPairOfPantsRepresentation: you gave me a negative length (seriously?)"));
    }
    if (normalized == c3)
    {
        Gamma.setPairOfPants(c1, c2, c3);

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
    }
    else if (normalized == c1)
    {
        setNormalizedPairOfPantsRepresentation(c2, c3, c1, C2, C3, C1, S2, S3, S1, c1);
        rotateGenerators(1);
    }
    else if (normalized == c2)
    {
        setNormalizedPairOfPantsRepresentation(c3, c1, c2, C3, C1, C2, S3, S1, S2, c2);
        rotateGenerators(2);
    }
    else
    {
        throw(QString("ERROR in GroupRepresentation<H2Isometry>::setNormalizedPairOfPantsRepresentation: Normalizer has to be one of the generators!"));
    }
}

template <> void GroupRepresentation<H2Isometry>::setNormalizedPairOfPantsRepresentation(generatorName c1, generatorName c2, generatorName c3,
                                                                                         double length1, double length2, double length3,
                                                                                         generatorName normalized)
{
    setNormalizedPairOfPantsRepresentation(c1, c2, c3, cosh(length1/2), cosh(length2/2), cosh(length3/2), sinh(length1/2), sinh(length2/2), sinh(length3/2), normalized);
}

template <> void GroupRepresentation<H2Isometry>::setNiceRepresentation()
{
    Gamma = DiscreteGroup(TopologicalSurface(2, 0));

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
}



template <> bool GroupRepresentation<H2Isometry>::checkCompatibilityOfFNcoordinates(const std::vector<double> & lengths,
                                                                                    const std::vector<double> & twists)
{
    uint N = lengths.size();
    if (twists.size() != N)
    {
        throw(QString("ERROR in GroupRepresentation<H2Isometry>::setFenchelNielsenCoordinatesUnnormalized: number of lengths and twists don't match!"));
        return false;
    }
    else if (N%3 != 0)
    {
        throw(QString("ERROR in GroupRepresentation<H2Isometry>::setFenchelNielsenCoordinatesUnnormalized: number of lengths and twists has to be a multiple of 3!"));
    }
    else if (N<3)
    {
        throw(QString("ERROR in GroupRepresentation<H2Isometry>::setFenchelNielsenCoordinatesUnnormalized: at least 3 lengths and twists are needed!"));
        return false;
    }
    return true;
}

template <> GroupRepresentation<H2Isometry> GroupRepresentation<H2Isometry>::amalgamateOverInverse(const GroupRepresentation<H2Isometry> & rho1, const generatorName &a1,
                                                                                                   const GroupRepresentation<H2Isometry> & rho2,
                                                                                                   const generatorName &a1inverse)
{
    std::vector<H2Isometry> generatorImages1 = rho1.getGeneratorImages();
    std::vector<H2Isometry> generatorImages2 = rho2.getGeneratorImages();

    generatorImages1.insert(generatorImages1.end(), generatorImages2.begin(), generatorImages2.end());

    return GroupRepresentation<H2Isometry>(DiscreteGroup::amalgamateOverInverse(rho1.Gamma, a1, rho2.Gamma, a1inverse), generatorImages1);
}


template <> GroupRepresentation<H2Isometry> GroupRepresentation<H2Isometry>::HNNextensionOverInverse(const GroupRepresentation<H2Isometry> &rho,
                                                                                                     const generatorName &a,
                                                                                                     const generatorName &ainverse,
                                                                                                     const generatorName &newGeneratorName,
                                                                                                     const H2Isometry & conjugator)
{
    std::vector<H2Isometry> outputGeneratorImages = rho.getGeneratorImages();
    outputGeneratorImages.push_back(conjugator);

    return GroupRepresentation<H2Isometry>(DiscreteGroup::HNNextensionOverInverse(rho.Gamma, a, ainverse, newGeneratorName), outputGeneratorImages);
}





template class GroupRepresentation<H2Isometry>;
template class GroupRepresentation<H3Isometry>;
template class GroupRepresentation<SL2RMatrix>;
template class GroupRepresentation<SL2CMatrix>;
