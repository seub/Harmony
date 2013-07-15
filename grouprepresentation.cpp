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
            std::cout <<  test.error() << std::endl;
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
        std::cout << "WARNING in RealRepresentation::generatePolygon: not a closed surface group representation" << std::endl;
    }
    std::cout << res << std::endl;
    return res;
}


template <> std::vector<H2Isometry> IsomH2Representation::getSidePairingsForNormalizedFundamentalDomain() const
{
    std::vector<H2Isometry> output;
    if (Gamma->isClosedSurfaceGroup())
    {
        int genus = generatorImages.size()/2;
        H2Isometry store;
        store.setIdentity();

        for (int i = 0; i<genus; i++)
        {

            output.push_back(store*generatorImages[2*i]*
                    generatorImages[2*i+1]*
                    generatorImages[2*i].inverse()*store.inverse());
            output.push_back(store*generatorImages[2*i]*generatorImages[2*i+1]*
                    generatorImages[2*i].inverse()*
                    generatorImages[2*i+1].inverse()*generatorImages[2*i].inverse()*store.inverse());
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

template <> IsomH2Representation IsomH2Representation::amalgamateOverInverse(DiscreteGroup *outputDiscreteGroup,
                                                                             const IsomH2Representation & rho1, const generatorName &a1,
                                                                             const IsomH2Representation & rho2,
                                                                             const generatorName &a1inverse,
                                                                             const H2Isometry & conjugator)
{
    *outputDiscreteGroup = DiscreteGroup::amalgamateOverInverse(*(rho1.Gamma), a1, *(rho2.Gamma), a1inverse);
    IsomH2Representation rho2new(rho2.Gamma);
    rho2new = rho2.conj(conjugator);

    std::vector<H2Isometry> generatorImages1 = rho1.getGeneratorImages();
    std::vector<H2Isometry> generatorImages2 = rho2new.getGeneratorImages();
    std::vector<H2Isometry> outputGeneratorImages = generatorImages1;

    outputGeneratorImages.insert(outputGeneratorImages.end(), generatorImages2.begin(), generatorImages2.end());

    return IsomH2Representation(outputDiscreteGroup, outputGeneratorImages);
}


template <> IsomH2Representation IsomH2Representation::amalgamateOverInverse(DiscreteGroup *outputDiscreteGroup,
                                                                             const IsomH2Representation & rho1, const generatorName &a1,
                                                                             const generatorName &a1left,
                                                                             const IsomH2Representation & rho2,
                                                                             const generatorName &a1inverse,
                                                                             const generatorName &a1inverseleft,
                                                                             double twist)
{
    H2Isometry f1, f1left, f2, f2left;

    if (!(rho1.getGeneratorImage(a1, f1) && rho1.getGeneratorImage(a1left, f1left) &&
          rho2.getGeneratorImage(a1inverse, f2) && rho2.getGeneratorImage(a1inverseleft, f2left)))
    {
        std::cout << "ERROR in IsomH2Representation::amalgamateOverInverse: generator name is not in the group!" << std::endl;
    }


    H2Isometry conjugator = H2Isometry::findConjugatorForGluing(f1, f1left, f2, f2left, twist);

    return IsomH2Representation::amalgamateOverInverse(outputDiscreteGroup, rho1, a1, rho2, a1inverse, conjugator);
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

template <> IsomH2Representation IsomH2Representation::doHNNextensionOverInverse(DiscreteGroup *outputDiscreteGroup,
                                                                                 const IsomH2Representation &rho,
                                                                                 const generatorName &a, const generatorName &aleft,
                                                                                 const generatorName &ainverse,
                                                                                 const generatorName &ainverseleft,
                                                                                 const generatorName &newGeneratorName, double twist)
{

    H2Isometry f1, f1left, f2, f2left;

    if (!(rho.getGeneratorImage(a, f1) && rho.getGeneratorImage(aleft, f1left) &&
          rho.getGeneratorImage(ainverse, f2) && rho.getGeneratorImage(ainverseleft, f2left)))
    {
        std::cout << "ERROR in IsomH2Representation::amalgamateOverInverse: generator name is not in the group!" << std::endl;
    }

    H2Isometry conjugator = H2Isometry::findConjugatorForGluing(f1, f1left, f2, f2left, twist);

   return doHNNextensionOverInverse(outputDiscreteGroup,rho,a,ainverse,newGeneratorName,conjugator);
}



template <> IsomH2Representation IsomH2Representation::setFNCoordinatesUnnormalized(DiscreteGroup* outputDiscreteGroup,
                                                                                    const std::vector<double> & lengths,
                                                                                    const std::vector<double> & twists)
{
    int N = lengths.size();
    int g = N/3 + 1;
    IsomH2Representation Output(outputDiscreteGroup);

    //std::vector<DiscreteGroup> PantGroups;
    std::vector<DiscreteGroup*> PantGroupsPointers;
    for (int m=0; m<2*g-2; m++)
    {
        DiscreteGroup * PantGroup = new DiscreteGroup;
        PantGroupsPointers.push_back(PantGroup);
    }

    std::vector<IsomH2Representation> rhos = IsomH2Representation::getPantsRepresentations(lengths, twists, PantGroupsPointers);


    Output = IsomH2Representation::amalgamateOverInverse(outputDiscreteGroup, rhos[0], "U1up", "W1up", rhos[1], "U1down", "W2up", twists[1]);

    if (g > 2)
    {
        Output = IsomH2Representation::amalgamateOverInverse(outputDiscreteGroup, Output, "V1up","U1down", rhos[2], "V1down", "W2down", twists[2]);
    }

    std::string s1,s2,s3,uUp,uDn,vUp,vDn,fuUp,fuDn,fvUp,fvDn;

    for(int j=1; j<g-2; j++)
    {
        s1 = Tools::convertToString(j);
        s2 = Tools::convertToString(j+1);
        s3 = Tools::convertToString(j+2);

        uUp.append("U").append(s2).append("up");
        uDn.append("U").append(s2).append("down");
        vUp.append("V").append(s2).append("up");
        vDn.append("V").append(s2).append("down");
        fuUp.append("V").append(s1).append("down");
        fuDn.append("W").append(s3).append("up");
        fvUp.append("U").append(s2).append("down");
        fvDn.append("W").append(s3).append("down");


        Output = IsomH2Representation::amalgamateOverInverse(outputDiscreteGroup, Output, uUp, fuUp,
                                                             rhos[2*j+1], uDn, fuDn, twists[3*j+1]);
        Output = IsomH2Representation::amalgamateOverInverse(outputDiscreteGroup, Output, vUp, fvUp,
                                                             rhos[2*j+2], vDn, fvDn, twists[3*j+2]);

        uUp.clear();
        uDn.clear();
        vUp.clear();
        vDn.clear();
        fuUp.clear();
        fuDn.clear();
        fvUp.clear();
        fvDn.clear();

    }


    if (g>2)
    {
        s1 = Tools::convertToString(g-1);
        s2 = Tools::convertToString(g);
        s3 = Tools::convertToString(g-2);

        uUp.append("U").append(s1).append("up");
        uDn.append("U").append(s1).append("down");
        fuUp.append("V").append(s3).append("down");
        fuDn.append("W").append(s2).append("down");

        Output = IsomH2Representation::amalgamateOverInverse(outputDiscreteGroup, Output, uUp, fuUp,
                                                             rhos[2*g-3], uDn, fuDn, twists[3*g-5]);
    }




    Output = IsomH2Representation::doHNNextensionOverInverse(outputDiscreteGroup,Output,"W1up","W1down","W1down","U1up","D1",twists[0]);

    std::string wUp,wDn,fwUp,fwDn,d;

    for(int k=0;k<g-2;k++)
    {
        s2 = Tools::convertToString(k+1);
        s3 = Tools::convertToString(k+2);

        wUp.append("W").append(s3).append("up");
        wDn.append("W").append(s3).append("down");

        fwUp.append("V").append(s2).append("up");
        fwDn.append("U").append(s3).append("up");
        d.append("D").append(s3);

        Output = IsomH2Representation::doHNNextensionOverInverse(outputDiscreteGroup,Output,wUp,fwUp,wDn,fwDn,d,twists[3*k+3]);

        wUp.clear();
        wDn.clear();
        fwUp.clear();
        fwDn.clear();
        d.clear();
    }
    s1 = Tools::convertToString(g-1);
    s2 = Tools::convertToString(g);

    wUp.append("W").append(s2).append("up");
    wDn.append("W").append(s2).append("down");

    fwUp.append("U").append(s1).append("down");
    fwDn.append("W").append(s2).append("up");
    d.append("D").append(s2);

    Output = IsomH2Representation::doHNNextensionOverInverse(outputDiscreteGroup,Output,wUp,fwUp,wDn,fwDn,d,twists[3*g-4]);


    /*for (unsigned int l = 0; l<PantGroupsPointers.size(); l++)
    {
        delete PantGroupsPointers[l];
    }*/
    //std::cout << "Leaving IsomH2Representation::setFNCoordinatesUnnormalized" << std::endl;

    return Output;
}




template <> IsomH2Representation IsomH2Representation::setFNCoordinatesUnnormalizedNew(DiscreteGroup* outputDiscreteGroup,
                                                                                    const std::vector<double> & lengths,
                                                                                    const std::vector<double> & twists)
{
    int N = lengths.size();
    int g = N/3 + 1;
    IsomH2Representation Output(outputDiscreteGroup);

    //std::vector<DiscreteGroup> PantGroups;
    std::vector<DiscreteGroup*> PantGroupsPointers;
    for (int m=0; m<2*g-2; m++)
    {
        DiscreteGroup * PantGroup = new DiscreteGroup;
        PantGroupsPointers.push_back(PantGroup);
    }

    std::vector<IsomH2Representation> rhos = IsomH2Representation::getPantsRepresentations(lengths, twists, PantGroupsPointers);

    std::vector<H2Isometry> conjugators;



    Output = IsomH2Representation::amalgamateOverInverse(outputDiscreteGroup, rhos[0], "U1up", "W1up", rhos[1], "U1down", "W2up", twists[1]);

    if (g > 2)
    {
        Output = IsomH2Representation::amalgamateOverInverse(outputDiscreteGroup, Output, "V1up","U1down", rhos[2], "V1down", "W2down", twists[2]);
    }

    std::string s1,s2,s3,uUp,uDn,vUp,vDn,fuUp,fuDn,fvUp,fvDn;

    for(int j=1; j<g-2; j++)
    {
        s1 = Tools::convertToString(j);
        s2 = Tools::convertToString(j+1);
        s3 = Tools::convertToString(j+2);

        uUp.append("U").append(s2).append("up");
        uDn.append("U").append(s2).append("down");
        vUp.append("V").append(s2).append("up");
        vDn.append("V").append(s2).append("down");
        fuUp.append("V").append(s1).append("down");
        fuDn.append("W").append(s3).append("up");
        fvUp.append("U").append(s2).append("down");
        fvDn.append("W").append(s3).append("down");


        Output = IsomH2Representation::amalgamateOverInverse(outputDiscreteGroup, Output, uUp, fuUp,
                                                             rhos[2*j+1], uDn, fuDn, twists[3*j+1]);
        Output = IsomH2Representation::amalgamateOverInverse(outputDiscreteGroup, Output, vUp, fvUp,
                                                             rhos[2*j+2], vDn, fvDn, twists[3*j+2]);

        uUp.clear();
        uDn.clear();
        vUp.clear();
        vDn.clear();
        fuUp.clear();
        fuDn.clear();
        fvUp.clear();
        fvDn.clear();

    }


    if (g>2)
    {
        s1 = Tools::convertToString(g-1);
        s2 = Tools::convertToString(g);
        s3 = Tools::convertToString(g-2);

        uUp.append("U").append(s1).append("up");
        uDn.append("U").append(s1).append("down");
        fuUp.append("V").append(s3).append("down");
        fuDn.append("W").append(s2).append("down");

        Output = IsomH2Representation::amalgamateOverInverse(outputDiscreteGroup, Output, uUp, fuUp,
                                                             rhos[2*g-3], uDn, fuDn, twists[3*g-5]);
    }




    Output = IsomH2Representation::doHNNextensionOverInverse(outputDiscreteGroup,Output,"W1up","W1down","W1down","U1up","D1",twists[0]);

    std::string wUp,wDn,fwUp,fwDn,d;

    for(int k=0;k<g-2;k++)
    {
        s2 = Tools::convertToString(k+1);
        s3 = Tools::convertToString(k+2);

        wUp.append("W").append(s3).append("up");
        wDn.append("W").append(s3).append("down");

        fwUp.append("V").append(s2).append("up");
        fwDn.append("U").append(s3).append("up");
        d.append("D").append(s3);

        Output = IsomH2Representation::doHNNextensionOverInverse(outputDiscreteGroup,Output,wUp,fwUp,wDn,fwDn,d,twists[3*k+3]);

        wUp.clear();
        wDn.clear();
        fwUp.clear();
        fwDn.clear();
        d.clear();
    }
    s1 = Tools::convertToString(g-1);
    s2 = Tools::convertToString(g);

    wUp.append("W").append(s2).append("up");
    wDn.append("W").append(s2).append("down");

    fwUp.append("U").append(s1).append("down");
    fwDn.append("W").append(s2).append("up");
    d.append("D").append(s2);

    Output = IsomH2Representation::doHNNextensionOverInverse(outputDiscreteGroup,Output,wUp,fwUp,wDn,fwDn,d,twists[3*g-4]);


    /*for (unsigned int l = 0; l<PantGroupsPointers.size(); l++)
    {
        delete PantGroupsPointers[l];
    }*/
    //std::cout << "Leaving IsomH2Representation::setFNCoordinatesUnnormalized" << std::endl;

    return Output;
}






template <> void IsomH2Representation::setFenchelNielsenCoordinates(const std::vector<double> & lengths,
                                                                    const std::vector<double> & twists)
{
    DiscreteGroup GammaUnnormalized;
    IsomH2Representation Unnormalized = IsomH2Representation::setFNCoordinatesUnnormalized(&GammaUnnormalized, lengths, twists);

    int g = lengths.size()/3 + 1;
    *Gamma = DiscreteGroup(TopologicalSurface(g, 0));

    std::vector<H2Isometry> OutputGeneratorImages;
    std::string Viup, Di, s;
    H2Isometry A, B;

    Unnormalized.getGeneratorImage("W1down", A);
    A = A.inverse();
    OutputGeneratorImages.push_back(A);

    Unnormalized.getGeneratorImage("D1", B);
    OutputGeneratorImages.push_back(B);

    for (int i = 1; i < g-1; i++)
    {
        s = Tools::convertToString(i);
        Viup.append("V").append(s).append("up");
        Unnormalized.getGeneratorImage(Viup, A);
        A = A.inverse();
        OutputGeneratorImages.push_back(A);

        s = Tools::convertToString(i+1);
        Di.append("D").append(s);
        Unnormalized.getGeneratorImage(Di, B);
        OutputGeneratorImages.push_back(B);

        Viup.clear();
        Di.clear();
    }

    s=Tools::convertToString(g);
    std::string Wgup, Dg;
    Wgup.append("W").append(s).append("up");

    Unnormalized.getGeneratorImage(Wgup, A);
    A = A.inverse();
    OutputGeneratorImages.push_back(A);

    Dg.append("D").append(s);
    Unnormalized.getGeneratorImage(Dg, B);
    B = B.inverse();
    OutputGeneratorImages.push_back(B);

    generatorImages = OutputGeneratorImages;
    return;
}








template class GroupRepresentation<H2Isometry>;
template class GroupRepresentation<H3Isometry>;
template class GroupRepresentation<SL2RMatrix>;
template class GroupRepresentation<SL2CMatrix>;
