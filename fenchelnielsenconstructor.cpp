#include "fenchelnielsenconstructor.h"
#include "canvas.h"
#include "h2canvasdelegate.h"


PantsTree::PantsTree(int index, const std::vector<double> &CoshHalfLengthsAugmented,
                     const std::vector<double> &SinhHalfLengthsAugmented, const std::string & genericCurveName) : index(index)
{
    rho = new IsomH2Representation(&Gamma);

    double Cup = CoshHalfLengthsAugmented[index-1];
    double Cleft = CoshHalfLengthsAugmented[2*index-1];
    double Cright = CoshHalfLengthsAugmented[2*index];
    double Sup = SinhHalfLengthsAugmented[index-1];
    double Sleft = SinhHalfLengthsAugmented[2*index-1];
    double Sright = SinhHalfLengthsAugmented[2*index];

    std::string sup = Tools::convertToString(index), sleft = Tools::convertToString(2*index), sright = Tools::convertToString(2*index + 1);
    generatorName up, left, right;
    up.append(genericCurveName).append(sup).append("down");
    left.append(genericCurveName).append(sleft).append("up");
    right.append(genericCurveName).append(sright).append("up");
    rho->setNormalizedPairOfPantsRepresentation(up, left, right, Cup, Cleft, Cright, Sup, Sleft, Sright, up);

    H2Isometry fRight;
    rho->getGeneratorImage(right, fRight);
    H2Geodesic L;
    fRight.axis(L);
    twistCorrection = H2Isometry::geodesicNormalizer(L);
}

PantsTreeNode::PantsTreeNode(int index, const std::vector<double> & coshHalfLengthsAugmented, const std::vector<double> & sinhHalfLengthsAugmented,
                             const std::vector<double> & twistsNormalized, const std::string & genericCurveName)
    : PantsTree(index, coshHalfLengthsAugmented, sinhHalfLengthsAugmented, genericCurveName)
{

    int N = twistsNormalized.size();
    if(8*index >= N)
    {
        leftChild = new PantsTreeLeaf(2*index, coshHalfLengthsAugmented, sinhHalfLengthsAugmented, twistsNormalized, genericCurveName);
    }
    else
    {
        leftChild = new PantsTreeNode(2*index, coshHalfLengthsAugmented, sinhHalfLengthsAugmented, twistsNormalized, genericCurveName);
    }

    if(8*index +4 >= N)
    {
        rightChild = new PantsTreeLeaf(2*index+1, coshHalfLengthsAugmented, sinhHalfLengthsAugmented, twistsNormalized, genericCurveName);
    }
    else
    {
        rightChild = new PantsTreeNode(2*index+1, coshHalfLengthsAugmented, sinhHalfLengthsAugmented, twistsNormalized, genericCurveName);
    }

    double twistCorrectionLeft = leftChild->twistCorrection, twistCorrectionright = rightChild->twistCorrection;
    std::vector<H2Isometry> upleftright = rho->getGeneratorImages();

    H2Isometry fUp = upleftright[0], fLeft = upleftright[1], fRight= upleftright[2];

    conjugatorLeft.setByNormalizingPairOnLeftHandSide(fLeft, fUp);
    H2Isometry twister;
    twister.setVerticalTranslation(-twistsNormalized[2*index-1], twistCorrectionLeft);
    conjugatorLeft = twister*conjugatorLeft;
    conjugatorLeft = conjugatorLeft.inverse();

    conjugatorRight.setByNormalizingPairOnLeftHandSide(fRight, fLeft);
    twister.setVerticalTranslation(-twistsNormalized[2*index], twistCorrectionright);
    conjugatorRight = twister*conjugatorRight;
    conjugatorRight = conjugatorRight.inverse();
    //std::cout << *rho << std::endl;
}

PantsTreeNode::~PantsTreeNode()
{
    delete leftChild;
    delete rightChild;
    delete rho;
}


IsomH2Representation PantsTreeNode::getRepresentation(DiscreteGroup *group, H2Isometry &totalConjugator, const std::string &genericCurveName)
{
    H2Isometry totalConjugatorLeft = totalConjugator*conjugatorLeft;
    H2Isometry totalConjugatorRight = totalConjugator*conjugatorRight;

    std::vector<generatorName> generators = Gamma.getGenerators();
    IsomH2Representation rhobis(&Gamma);
    rhobis = (*rho).conjugate(totalConjugator);
    DiscreteGroup leftgroup, rightgroup;
    IsomH2Representation leftrho = leftChild->getRepresentation(&leftgroup,totalConjugatorLeft, genericCurveName);
    IsomH2Representation rightrho = rightChild->getRepresentation(&rightgroup,totalConjugatorRight, genericCurveName);
    DiscreteGroup tempGroup;
    IsomH2Representation temprho = IsomH2Representation::amalgamateOverInverse(&tempGroup,rhobis,generators[1],leftrho,
            leftChild->Gamma.getGenerators()[0]);
    return IsomH2Representation::amalgamateOverInverse(group,temprho,generators[2],rightrho,rightChild->Gamma.getGenerators()[0]);
}

PantsTreeLeaf::PantsTreeLeaf(int index, const std::vector<double> &coshHalfLengthsAugmented, const std::vector<double> &sinhHalfLengthsAugmented,
                             const std::vector<double> &twistsNormalized, const std::string & genericCurveName) :
    PantsTree(index, coshHalfLengthsAugmented, sinhHalfLengthsAugmented, genericCurveName)
{
    std::vector<H2Isometry> upleftright = rho->getGeneratorImages();
    H2Isometry fLeft = upleftright[1], fUp = upleftright[0], fRight= upleftright[2];
    hNNconjugator = H2Isometry::findConjugatorForGluing(fRight, fLeft, fLeft, fUp, twistsNormalized[2*index-1]);
    //std::cout << *rho << std::endl;
}

PantsTreeLeaf::~PantsTreeLeaf()
{
    delete rho;
}

IsomH2Representation PantsTreeLeaf::getRepresentation(DiscreteGroup *group, H2Isometry &totalConjugator, const std::string & genericCurveName)
{
    std::string s = Tools::convertToString(index);
    generatorName stableLetter;
    stableLetter.append(genericCurveName).append("s").append(s);
    std::vector<generatorName> generators = Gamma.getGenerators();

    return (IsomH2Representation::doHNNextensionOverInverse(group,*rho,generators[2],
            generators[1],stableLetter,hNNconjugator)).conjugate(totalConjugator);
}

FenchelNielsenConstructor::FenchelNielsenConstructor(const std::vector<double> &lengths, const std::vector<double> &twists)
{
    this->lengths = lengths;
    this->twists = twists;
    genus=lengths.size()/3 + 1;
    gLeft = genus/2;
    gRight = genus - gLeft;

    setNormalizedLengths();
    setNormalizedTwists();
    splitAugmentedLengthsAndTwists();


    firstTwist = nTwists[0];
    if(gLeft>1)
    {
        LeftTree = new PantsTreeNode(1, cLengthsLeft, sLengthsLeft, nTwistsLeft, "c");

    }
    else
    {
        LeftTree = new PantsTreeLeaf(1, cLengthsLeft, sLengthsLeft, nTwistsLeft, "c");
    }

    if(gRight>1)
    {
        RightTree = new PantsTreeNode(1, cLengthsRight, sLengthsRight, nTwistsRight, "d");
    }
    else
    {
        RightTree = new PantsTreeLeaf(1, cLengthsRight, sLengthsRight, nTwistsRight, "d");
    }

}

FenchelNielsenConstructor::~FenchelNielsenConstructor()
{
    delete LeftTree;
    delete RightTree;
}

void FenchelNielsenConstructor::setNormalizedTwists()
{
    double l, twistOut;
    nTwists.clear();
    nTwists.reserve(twists.size());
    for (auto twist : twists)
    {
        l = cosh(twist);
        twistOut = sqrt((l-1.0)/(l+1.0));
        if (twist < 0)
        {
            twistOut = -twistOut;
        }
        nTwists.push_back(twistOut);
    }
}

void FenchelNielsenConstructor::setNormalizedLengths()
{
    cLengths.clear();
    sLengths.clear();
    cLengths.reserve(lengths.size());
    sLengths.reserve(lengths.size());

    for (const auto length : lengths)
    {
        cLengths.push_back(cosh(0.5*length));
        sLengths.push_back(sinh(0.5*length));
    }
}

void FenchelNielsenConstructor::splitAugmentedLengthsAndTwists()
{
    for (int i1=0; i1<2*gLeft - 1; i1++)
    {
        cLengthsLeft.push_back(cLengths[i1]);
        sLengthsLeft.push_back(sLengths[i1]);
        nTwistsLeft.push_back(nTwists[i1]);
    }

    // what follows is for the leaves
    for (int i2=2*gLeft-1; i2<3*gLeft - 1; i2++)
    {
        cLengthsLeft.push_back(cLengths[i2]);
        sLengthsLeft.push_back(sLengths[i2]);
        nTwistsLeft.push_back(nTwists[i2]);
        cLengthsLeft.push_back(cLengths[i2]);
        sLengthsLeft.push_back(sLengths[i2]);
        nTwistsLeft.push_back(nTwists[i2]);
    }


    cLengthsRight.push_back(cLengthsLeft[0]);
    sLengthsRight.push_back(sLengthsLeft[0]);
    nTwistsRight.push_back(nTwists[0]);

    for (int j1=0; j1<2*gRight - 2; j1++)
    {
        cLengthsRight.push_back(cLengths[j1 + 3*gLeft - 1]);
        sLengthsRight.push_back(sLengths[j1 + 3*gLeft - 1]);
        nTwistsRight.push_back(nTwists[j1 + 3*gLeft - 1]);
    }

    // what follows is for the leaves
    for (int j2=2*gRight-2; j2<3*gRight - 2; j2++)
    {
        cLengthsRight.push_back(cLengths[j2 + 3*gLeft - 1]);
        sLengthsRight.push_back(sLengths[j2 + 3*gLeft - 1]);
        nTwistsRight.push_back(nTwists[j2 + 3*gLeft - 1]);
        cLengthsRight.push_back(cLengths[j2 + 3*gLeft - 1]);
        sLengthsRight.push_back(sLengths[j2 + 3*gLeft - 1]);
        nTwistsRight.push_back(nTwists[j2 + 3*gLeft -1]);
    }

    std::cout << "twists  = " << nTwists << std::endl;
    std::cout << "twists left = " << nTwistsLeft << std::endl;
    std::cout << "twists right = " << nTwistsRight << std::endl;

}

IsomH2Representation FenchelNielsenConstructor::getUnnormalizedRepresentation(DiscreteGroup *group)
{
    H2Isometry id;
    id.setIdentity();

    DiscreteGroup GammaLeft, GammaRight;



    /*H2Isometry twister;
    twister.setVerticalTranslation(firstTwist, LeftTree->twistCorrection, - RightTree->twistCorrection);


    H2Isometry halfTurn;
    halfTurn.setDiskCoordinates(-1.0, 0.0);*/

    H2Isometry c, c2;
    c.setVerticalTranslation(firstTwist);
    H2Isometry halfTurn;
    halfTurn.setDiskCoordinates(-1.0, 0.0);
    c2.setVerticalTranslation(RightTree->twistCorrection);
    c2 = halfTurn*c2;


    H2Isometry totalConjugatorRight = c*c2;
    H2Isometry totalConjugatorLeft;
    totalConjugatorLeft.setVerticalTranslation(LeftTree->twistCorrection);


    IsomH2Representation rhoLeft = LeftTree->getRepresentation(&GammaLeft, totalConjugatorLeft, "c");
    IsomH2Representation rhoRight = RightTree->getRepresentation(&GammaRight, totalConjugatorRight, "d");


    return IsomH2Representation::amalgamateOverInverse(group, rhoLeft, "c1down", rhoRight, "d1down");
}

IsomH2Representation FenchelNielsenConstructor::getRepresentation(DiscreteGroup *group)
{
    DiscreteGroup group1;
    IsomH2Representation rhoU = getUnnormalizedRepresentation(&group1);

    IsomH2Representation rho(group);
    *group = DiscreteGroup(TopologicalSurface(genus, 0));

    std::string s;
    generatorName ai, bi;
    unsigned int gleft = genus/2;
    unsigned int gright = genus - gleft;
    std::vector<H2Isometry> rhoIsometry;
    H2Isometry tempIsom;

    unsigned int powerOfTwo=1;

    while(powerOfTwo<gright)
    {
        powerOfTwo*=2;
    }
    for(unsigned int i = powerOfTwo ; i<2*gleft;i++)
    {
        s = Tools::convertToString(i);
        ai = "c";
        ai.append("s").append(s);
        rhoU.getGeneratorImage(ai,tempIsom);
        rhoIsometry.push_back(tempIsom.inverse());

        s = Tools::convertToString(2*(i)+1);
        bi = "c";
        bi.append(s).append("up");
        rhoU.getGeneratorImage(bi,tempIsom);
        rhoIsometry.push_back(tempIsom);

    }
    for(unsigned int i = gleft ; i<powerOfTwo;i++)
    {
        s = Tools::convertToString(i);
        ai = "c";
        ai.append("s").append(s);
        rhoU.getGeneratorImage(ai,tempIsom);
        rhoIsometry.push_back(tempIsom.inverse());

        s = Tools::convertToString(2*(i)+1);
        bi = "c";
        bi.append(s).append("up");
        rhoU.getGeneratorImage(bi,tempIsom);
        rhoIsometry.push_back(tempIsom);

    }
    powerOfTwo=1;
    while(powerOfTwo<gright)
    {
        powerOfTwo*=2;
    }

    for(unsigned int i = powerOfTwo ; i<2*gright;i++)
    {

        s = Tools::convertToString(i);
        bi = "d";
        bi.append("s").append(s);
        if(!rhoU.getGeneratorImage(bi,tempIsom))
        {
            throw(QString("Error in FenchelNielsenConstructor::getRepresentation"));
        }
        rhoIsometry.push_back(tempIsom.inverse());

        s = Tools::convertToString(2*(i)+1);
        ai = "d";
        ai.append(s).append("up");
        if(!rhoU.getGeneratorImage(ai,tempIsom))
        {
            throw(QString("Error in FenchelNielsenConstructor::getRepresentation"));
        }
        rhoIsometry.push_back(tempIsom);
    }
    for(unsigned int i = gright ; i<powerOfTwo;i++)
    {

        s = Tools::convertToString(i);
        bi = "d";
        bi.append("s").append(s);
        if(!rhoU.getGeneratorImage(bi,tempIsom))
        {
            throw(QString("Error in FenchelNielsenConstructor::getRepresentation"));
        }
        rhoIsometry.push_back(tempIsom.inverse());

        s = Tools::convertToString(2*(i)+1);
        ai = "d";
        ai.append(s).append("up");
        if(!rhoU.getGeneratorImage(ai,tempIsom))
        {
            throw(QString("Error in FenchelNielsenConstructor::getRepresentation"));
        }
        rhoIsometry.push_back(tempIsom);


    }
    rho.generatorImages = rhoIsometry;
    return rho;
}
