#ifndef FENCHELNIELSENCONSTRUCTOR_H
#define FENCHELNIELSENCONSTRUCTOR_H


#include "tools.h"
#include "grouprepresentation.h"
#include "topologicalsurface.h"



class PantsTree
{
    friend class PantsTreeNode;
    friend class PantsTreeRoot;
public:
    PantsTree(int index, const std::vector<double> & lengths, bool root);
    virtual ~PantsTree() {}
    virtual bool isLeaf() const = 0;
    virtual IsomH2Representation getRepresentation(DiscreteGroup *group, H2Isometry &totalConjugator) = 0;
protected:
    DiscreteGroup Gamma;
    IsomH2Representation *rho;
    int index;
    double twistCorrection;
};

class PantsTreeNode : public PantsTree
{
public:
    PantsTreeNode(int index, const std::vector<double> & lengths, const std::vector<double> & twists);
    ~PantsTreeNode();

    bool isLeaf() const {return false;}
    bool isRoot() const {return false;}
    IsomH2Representation  getRepresentation(DiscreteGroup *group, H2Isometry &totalConjugator);
private:
    PantsTree *leftChild;
    PantsTree *rightChild;
    H2Isometry conjugatorLeft;
    H2Isometry conjugatorRight;
};

class PantsTreeLeaf : public PantsTree
{
public:
    PantsTreeLeaf(int index, const std::vector<double> & lengths, const std::vector<double> & twists);
    ~PantsTreeLeaf();
    bool isLeaf() const {return true;}
    IsomH2Representation getRepresentation(DiscreteGroup *group, H2Isometry &totalConjugator);
private:
    H2Isometry hNNconjugator;
};


class PantsTreeRoot: public PantsTree
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
};

class FenchelNielsenConstructor
{
public:
    FenchelNielsenConstructor(const std::vector<double> & lengths, const std::vector<double> & twists);
    ~FenchelNielsenConstructor();
    IsomH2Representation getrepresentation(DiscreteGroup *group);

private:
    PantsTreeRoot * tree;
};



#endif // FENCHELNIELSENCONSTRUCTOR_H
