#ifndef FENCHELNIELSENCONSTRUCTOR_H
#define FENCHELNIELSENCONSTRUCTOR_H


#include "tools.h"
#include "grouprepresentation.h"
#include "topologicalsurface.h"


class PantsTree;

class FenchelNielsenConstructor
{
public:
    FenchelNielsenConstructor(const std::vector<double> & lengths, const std::vector<double> & twists);
    ~FenchelNielsenConstructor();
    FenchelNielsenConstructor() = delete;
    FenchelNielsenConstructor(const FenchelNielsenConstructor &) = delete;
    FenchelNielsenConstructor & operator=(FenchelNielsenConstructor) = delete;

    GroupRepresentation<H2Isometry> getUnnormalizedRepresentation();
    GroupRepresentation<H2Isometry> getRepresentation();

private:
    void setNormalizedTwists();
    void setNormalizedLengths();
    void splitAugmentedLengthsAndTwists();

    uint genus, gLeft, gRight;
    double firstTwist;
    std::vector<double> lengths, cLengths, sLengths, cLengthsLeft, cLengthsRight, sLengthsLeft, sLengthsRight;
    std::vector<double> twists, nTwists, nTwistsLeft, nTwistsRight;
    PantsTree * LeftTree;
    PantsTree * RightTree;
};





class PantsTree
{
    friend class PantsTreeNode;
    friend class PantsTreeRoot;
    friend class FenchelNielsenConstructor;

public:
    PantsTree(uint index, const std::vector<double> & CoshHalfLengthsAugmented,
              const std::vector<double> &SinhHalfLengthsAugmented, const std::string & genericCurveName);
    virtual ~PantsTree() {}
    virtual GroupRepresentation<H2Isometry> getRepresentation(H2Isometry &totalConjugator, const std::string & genericCurveName) = 0;

protected:
    GroupRepresentation<H2Isometry> rho;
    uint index;
    double twistCorrection;

private:
    PantsTree(); //Dummy constructor
    PantsTree(const PantsTree & other); //Copy constructor
};

class PantsTreeNode : public PantsTree
{
public:
    PantsTreeNode(uint index, const std::vector<double> & coshHalfLengthsAugmented, const std::vector<double> & sinhHalfLengthsAugmented,
                  const std::vector<double> & twistsNormalized, const std::string & genericCurveName);
    ~PantsTreeNode();

    GroupRepresentation<H2Isometry> getRepresentation(H2Isometry &totalConjugator, const std::string & genericCurveName);
private:
    PantsTree *leftChild;
    PantsTree *rightChild;
    H2Isometry conjugatorLeft;
    H2Isometry conjugatorRight;

    PantsTreeNode(); // Dummy constructor
    PantsTreeNode(const PantsTreeNode &other); // Copy constructor
    PantsTreeNode & operator=(PantsTreeNode other); // Copy-assignment operator
};

class PantsTreeLeaf : public PantsTree
{
public:
    PantsTreeLeaf(uint index, const std::vector<double> & coshHalfLengthsAugmented,
                  const std::vector<double> & sinhHalfLengthsAugmented, const std::vector<double> & twistsNormalized,
                  const std::string &genericCurveName);
    ~PantsTreeLeaf();
    GroupRepresentation<H2Isometry> getRepresentation(H2Isometry &totalConjugator, const std::string &genericCurveName);
private:
    H2Isometry hNNconjugator;
    PantsTreeLeaf(); // Dummy constructor
    PantsTreeLeaf(const PantsTreeLeaf &other); // Copy constructor
    PantsTreeLeaf & operator=(PantsTreeLeaf other); // Copy-assignment operator
};


#endif // FENCHELNIELSENCONSTRUCTOR_H
