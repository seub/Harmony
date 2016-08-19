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

    void iterate(int flowChoice);
    void iterate(int flowChoice, uint nbIterations);
    void getOutputFunction(LiftedGraphFunction<Point, Map> *outputFunction);
    double updateSupDelta();
    void reset();


    Point getValue(uint index) const {return newValues.at(index);}

protected:
    void refreshNeighborsValuesKicked();
    void updateValuesCentroid();
    void refreshOutput();
    void updateValuesEnergyConstantStep();
    void updateValuesEnergyOptimalStep();
    void computeGradient();

    void lineSearch();

    //double computeEnergyHessian(const std::vector<H2TangentVector> &V);

    std::vector<H2TangentVector> gradient;
    double constantStep, optimalStep;

    const uint nbBoundaryPoints;
    const uint nbPoints;
    const std::vector< std::vector<uint> > neighborsIndices;
    const std::vector< std::vector<double> > neighborsWeightsCentroid,neighborsWeightsEnergy;
    const std::vector< std::vector<Map> > boundaryPointsNeighborsPairingsValues;

    std::vector<Point> initialValues, oldValues, newValues;
    std::vector< std::vector<Point> > neighborsValuesKicked;

    const std::unique_ptr<LiftedGraphFunction<Point, Map> > outputFunction;

    double supDelta;
    std::vector<double> errors;
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




#endif // DISCRETEFLOWITERATOR_H
