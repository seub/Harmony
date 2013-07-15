#ifndef FENCHELNIELSENCONSTRUCTOR_H
#define FENCHELNIELSENCONSTRUCTOR_H


#include "tools.h"
#include "grouprepresentation.h"
#include "topologicalsurface.h"



class PantsTree
{
    friend class PantsTreeNode;
    friend class PantsTreeRoot;
    friend class FenchelNielsenConstructor;
public:
    PantsTree(int index, const std::vector<double> & CoshHalfLengthsAugmented,
              const std::vector<double> &SinhHalfLengthsAugmented, const std::string & genericCurveName);
    virtual ~PantsTree() {}
    virtual IsomH2Representation getRepresentation(DiscreteGroup *group, H2Isometry &totalConjugator, const std::string & genericCurveName) = 0;
protected:
    DiscreteGroup Gamma;
    IsomH2Representation *rho;
    int index;
    double twistCorrection;
};

class PantsTreeNode : public PantsTree
{
public:
    PantsTreeNode(int index, const std::vector<double> & coshHalfLengthsAugmented, const std::vector<double> & sinhHalfLengthsAugmented,
                  const std::vector<double> & twistsNormalized, const std::string & genericCurveName);
    ~PantsTreeNode();

    IsomH2Representation getRepresentation(DiscreteGroup *group, H2Isometry &totalConjugator, const std::string & genericCurveName);
private:
    PantsTree *leftChild;
    PantsTree *rightChild;
    H2Isometry conjugatorLeft;
    H2Isometry conjugatorRight;
};

class PantsTreeLeaf : public PantsTree
{
public:
    PantsTreeLeaf(int index, const std::vector<double> & coshHalfLengthsAugmented,
                  const std::vector<double> & sinhHalfLengthsAugmented, const std::vector<double> & twistsNormalized,
                  const std::string &genericCurveName);
    ~PantsTreeLeaf();
    IsomH2Representation getRepresentation(DiscreteGroup *group, H2Isometry &totalConjugator, const std::string &genericCurveName);
private:
    H2Isometry hNNconjugator;
};


class FenchelNielsenConstructor
{
public:
    FenchelNielsenConstructor(const std::vector<double> & lengths, const std::vector<double> & twists);
    ~FenchelNielsenConstructor();

    IsomH2Representation getUnnormalizedRepresentation(DiscreteGroup *group);
    IsomH2Representation getRepresentation(DiscreteGroup *group);

private:
    int genus;
    double firstTwist;
    PantsTree * LeftTree;
    PantsTree * RightTree;
};



#endif // FENCHELNIELSENCONSTRUCTOR_H
