#include "fenchelnielsenconstructor.h"
#include "canvas.h"
#include "h2canvasdelegate.h"


PantsTree::PantsTree(int index, const std::vector<double> &lengths, bool root) : index(index)
{
    rho = new IsomH2Representation(&Gamma);

    if (!root)
    {
        double Lup = lengths[index];
        double Lleft = lengths[2*index];
        double Lright = lengths[2*index +1];

        std::string sup = Tools::convertToString(index), sleft = Tools::convertToString(2*index), sright = Tools::convertToString(2*index + 1);
        generatorName up, left, right;
        up.append("c").append(sup).append("down");
        left.append("c").append(sleft).append("up");
        right.append("c").append(sright).append("up");
        rho->setNormalizedPairOfPantsRepresentation(up, left, right, Lup, Lleft, Lright, up);

        H2Isometry fRight;
        rho->getGeneratorImage(right, fRight);
        H2Geodesic L;
        fRight.axis(L);
        H2Point p0, p;
        p0.setDiskCoordinate(0.0);
        L.closestPoint(H2Geodesic(-I, I), L, p);
        double d = H2distance(p0,p);
        twistCorrection = imag(p.getDiskCoordinate())>0 ? d : -d;
    }
}

PantsTreeNode::PantsTreeNode(int index, const std::vector<double> &lengths, const std::vector<double> &twists) : PantsTree(index, lengths, false)
{
    int N = lengths.size();
    if(8*index >= N)
    {
        leftChild = new PantsTreeLeaf(2*index,lengths, twists);
    }
    else
    {
        leftChild = new PantsTreeNode(2*index,lengths, twists);
    }

    if(8*index +4 >= N)
    {
        rightChild = new PantsTreeLeaf(2*index+1,lengths, twists);
    }
    else
    {
        rightChild = new PantsTreeNode(2*index+1,lengths, twists);
    }

    double twistCorrectionLeft = leftChild->twistCorrection, twistCorrectionright = rightChild->twistCorrection;
    std::vector<H2Isometry> upleftright = rho->getGeneratorImages();

    H2Isometry fUp = upleftright[0], fLeft = upleftright[1], fRight= upleftright[2];

    conjugatorLeft.setByNormalizingPairWithChosenNearestPointToAxis(fLeft, fUp, 0.0);
    H2Isometry twister;
    twister.setTranslationLengthNormalized(twists[2*index] + twistCorrectionLeft);
    conjugatorLeft = twister*conjugatorLeft;
    conjugatorLeft = conjugatorLeft.inverse();

    conjugatorRight.setByNormalizingPairWithChosenNearestPointToAxis(fRight, fLeft, 0.0);
    twister.setTranslationLengthNormalized(twists[2*index+1] + twistCorrectionright);
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
IsomH2Representation PantsTreeNode::getRepresentation(DiscreteGroup *group, H2Isometry &totalConjugator)
{
    H2Isometry totalConjugatorLeft = totalConjugator*conjugatorLeft;
    H2Isometry totalConjugatorRight = totalConjugator*conjugatorRight;

    std::vector<generatorName> generators = Gamma.getGenerators();
    IsomH2Representation rhobis(&Gamma);
    rhobis = (*rho).conj(totalConjugator);
    DiscreteGroup leftgroup, rightgroup;
    IsomH2Representation leftrho = leftChild->getRepresentation(&leftgroup,totalConjugatorLeft);
    IsomH2Representation rightrho = rightChild->getRepresentation(&rightgroup,totalConjugatorRight);
    DiscreteGroup tempGroup;
    IsomH2Representation temprho = IsomH2Representation::amalgamateOverInverse(&tempGroup,rhobis,generators[1],leftrho,
            leftChild->Gamma.getGenerators()[0]);
    return IsomH2Representation::amalgamateOverInverse(group,temprho,generators[2],rightrho,rightChild->Gamma.getGenerators()[0]);
}

PantsTreeLeaf::PantsTreeLeaf(int index, const std::vector<double> &lengths, const std::vector<double> &twists) : PantsTree(index, lengths, false)
{
    std::vector<H2Isometry> upleftright = rho->getGeneratorImages();
    H2Isometry fLeft = upleftright[1], fUp = upleftright[0], fRight= upleftright[2];
    hNNconjugator = H2Isometry::findConjugatorForGluing(fRight, fLeft, fLeft, fUp, twists[2*index]);
    //std::cout << *rho << std::endl;
}

PantsTreeLeaf::~PantsTreeLeaf()
{
    delete rho;
}

IsomH2Representation  PantsTreeLeaf::getRepresentation(DiscreteGroup *group, H2Isometry &totalConjugator)
{
    std::string s = Tools::convertToString(index);
    generatorName stableLetter;
    stableLetter.append("b").append(s);
    std::vector<generatorName> generators = Gamma.getGenerators();

    IsomH2Representation test = IsomH2Representation::doHNNextensionOverInverse(group,*rho,generators[2],
            generators[1],stableLetter,hNNconjugator);
    test.checkRelations();
    test = test.conj(totalConjugator);

    return (IsomH2Representation::doHNNextensionOverInverse(group,*rho,generators[2],
            generators[1],stableLetter,hNNconjugator)).conj(totalConjugator);
}

PantsTreeRoot::PantsTreeRoot(const std::vector<double> &lengths, const std::vector<double> &twists) : PantsTree(0, lengths, true)
{
    index =0;

    double Lupleft = lengths[0];
    double Lupright = lengths[0];
    double Ldown = lengths[1];

    rho->setNormalizedPairOfPantsRepresentation("c1up", "c0right", "c0left", Ldown, Lupright, Lupleft, "c1up");

    int N = lengths.size();
    if(4 >= N)
    {
        child = new PantsTreeLeaf(1, lengths, twists);
    }
    else
    {
        child = new PantsTreeNode(1, lengths, twists);
    }

    std::vector<H2Isometry> downrightleft = rho->getGeneratorImages();
    H2Isometry fDown = downrightleft[0], fRight = downrightleft[1], fLeft= downrightleft[2];
    hNNconjugator = H2Isometry::findConjugatorForGluing(fLeft, fRight, fRight, fDown, twists[0]);




    conjugator.setDiskCoordinates(-1.0, 0.0);
    H2Isometry twister;
    twister.setTranslationLengthNormalized(twists[1] + child->twistCorrection);
    conjugator = twister * conjugator;

    //std::cout << *rho << std::endl;
}

PantsTreeRoot::~PantsTreeRoot()
{
    delete child;
    delete rho;
}

IsomH2Representation  PantsTreeRoot::getRepresentation(DiscreteGroup *group, H2Isometry &)
{
    std::string s = Tools::convertToString(index);
    generatorName stableLetter;
    stableLetter.append("b").append(s);
    std::vector<generatorName> generators = Gamma.getGenerators();
    DiscreteGroup tempGroup, childGroup;
    IsomH2Representation tempRho = (IsomH2Representation::doHNNextensionOverInverse(&tempGroup,*rho,generators[2],generators[1],
            stableLetter,hNNconjugator)).conj(conjugator);
    H2Isometry id;
    id.setIdentity();
    IsomH2Representation childRho = child->getRepresentation(&childGroup,id);
    return IsomH2Representation::amalgamateOverInverse(group,tempRho,"c1up",childRho,"c1down");
}

FenchelNielsenConstructor::FenchelNielsenConstructor(const std::vector<double> &lengths, const std::vector<double> &twists)
{
    std::vector<double> lengthsAugmented = lengths;
    std::vector<double> twistsAugmented = twists;
    int N = lengths.size();
    int g = N/3 +1;

    lengthsAugmented.erase(lengthsAugmented.end() - g +1 ,lengthsAugmented.end());
    twistsAugmented.erase(twistsAugmented.end() - g +1,twistsAugmented.end());
    for(int i = N-g+1 ; i<N ; i++)
    {
        lengthsAugmented.push_back(lengths[i]);
        lengthsAugmented.push_back(lengths[i]);
        twistsAugmented.push_back(twists[i]);
        twistsAugmented.push_back(twists[i]);
    }
    tree = new PantsTreeRoot(lengthsAugmented, twistsAugmented);
}


FenchelNielsenConstructor::~FenchelNielsenConstructor()
{
    delete tree;
}

IsomH2Representation FenchelNielsenConstructor::getrepresentation(DiscreteGroup *group)
{
    H2Isometry id;
    id.setIdentity();
    return tree->getRepresentation(group,id);
}
