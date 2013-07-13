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
    PantsTree(int index, const std::vector<double> & lengths, const std::string & genericCurveName);
    virtual ~PantsTree() {}
    virtual bool isLeaf() const = 0;
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
    PantsTreeNode(int index, const std::vector<double> & lengths, const std::vector<double> & twists, const std::string & genericCurveName);
    ~PantsTreeNode();

    bool isLeaf() const {return false;}
    bool isRoot() const {return false;}
    IsomH2Representation  getRepresentation(DiscreteGroup *group, H2Isometry &totalConjugator, const std::string & genericCurveName);
private:
    PantsTree *leftChild;
    PantsTree *rightChild;
    H2Isometry conjugatorLeft;
    H2Isometry conjugatorRight;
};

class PantsTreeLeaf : public PantsTree
{
public:
    PantsTreeLeaf(int index, const std::vector<double> & lengths, const std::vector<double> & twists, const std::string &genericCurveName);
    ~PantsTreeLeaf();
    bool isLeaf() const {return true;}
    IsomH2Representation getRepresentation(DiscreteGroup *group, H2Isometry &totalConjugator, const std::string &genericCurveName);
private:
    H2Isometry hNNconjugator;
};


/*class PantsTreeRoot: public PantsTree
{

public:
    PantsTreeRoot(const std::vector<double> & lengths, const std::vector<double> & twists);
    ~PantsTreeRoot();
    IsomH2Representation getRepresentation(DiscreteGroup *group, H2Isometry &);
    bool isLeaf() const {return false;}
private:
    H2Isometry hNNconjugator;
    H2Isometry conjugator;
    PantsTree *child;
};*/




class FenchelNielsenConstructor
{
public:
    FenchelNielsenConstructor(const std::vector<double> & lengths, const std::vector<double> & twists);
    ~FenchelNielsenConstructor();

    IsomH2Representation getUnnormalizedRepresentation(DiscreteGroup *group);
    IsomH2Representation getNormalizedRepresentationJonah(DiscreteGroup *group);

private:
    int genus;
    double firstTwist;
    PantsTreeNode * LeftTree;
    PantsTreeNode * RightTree;
};



#endif // FENCHELNIELSENCONSTRUCTOR_H
