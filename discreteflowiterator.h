#ifndef DISCRETEFLOWITERATOR_H
#define DISCRETEFLOWITERATOR_H

#include "tools.h"
#include "h2tangentvector.h"

template<typename Point, typename Map> class LiftedGraphFunction;

template<typename Point, typename Map>
class DiscreteFlowIterator
{
public:
    DiscreteFlowIterator(const LiftedGraphFunction<Point, Map> *initialFunction);
    virtual ~DiscreteFlowIterator() {}

    void iterate();
    void iterate(uint nbIterations);
    void getOutputFunction(LiftedGraphFunction<Point, Map> *outputFunction);
    double updateSupDelta();
    virtual void reset();

    Point getValue(uint index) const {return newValues.at(index);}

protected:
    void refreshNeighborsValuesKicked();
    virtual void updateValues() = 0;
    void refreshOutput();

    const uint nbBoundaryPoints;
    const uint nbPoints;
    const std::vector< std::vector<uint> > neighborsIndices;
    const std::vector< std::vector<double> > neighborsWeights;
    const std::vector< std::vector<Map> > boundaryPointsNeighborsPairingsValues;

    std::vector<Point> initialValues, oldValues, newValues;
    std::vector< std::vector<Point> > neighborsValuesKicked;

    const std::unique_ptr<LiftedGraphFunction<Point, Map> > outputFunction;

    double supDelta;
    std::vector<double> errors;
};

template <typename Point, typename Map> class DiscreteFlowIteratorCentroid : public DiscreteFlowIterator<Point,Map>
{

public:
    DiscreteFlowIteratorCentroid(const LiftedGraphFunction<Point, Map> *initialFunction);
// Do copy constr etc as in liftedgraph class

private:
    virtual void updateValues() override;
};


/*template <typename Point, typename Map> class LiftedGraphFunctionTriangulated; template <typename Map> class GroupRepresentation;

template <typename Point, typename Map> class DiscreteHeatFlowIteratorRecursiveDepth
{
public:
    DiscreteHeatFlowIteratorRecursiveDepth(const GroupRepresentation<Map> &rhoDomain, const GroupRepresentation<Map> &rhoImage, uint depth);

    bool run();

private:
    GroupRepresentation<Map> rhoDomain, rhoImage;
    uint depth;

    double tol, subTol;

    std::vector< LiftedGraphFunctionTriangulated<Point, Map> > domainFunctions, imageFunctions;

    bool stop;
};*/


template <typename Point, typename Map> class DiscreteFlowIteratorEnergy : public DiscreteFlowIterator<Point,Map>
{
public:
    DiscreteFlowIteratorEnergy(const LiftedGraphFunction<Point, Map> *initialFunction);
    void reset();

private:
    virtual void updateValues() override;
    void constantStepUpdateValues();
    void optimalStepUpdateValues();
    void computeGradient();

//    double computeEnergyHessian(const std::vector<H2TangentVector> &V);


    void lineSearch();

    std::vector<H2TangentVector> gradient;
    double constantStep, optimalStep;
};

#endif // DISCRETEFLOWITERATOR_H
