#ifndef DISCRETEGRADIENTFLOW_H
#define DISCRETEGRADIENTFLOW_H

#include "tools.h"
#include "h2tangentvector.h"


template <typename Point, typename Map> class LiftedGraphFunction;
//class H2TangentVector;

template <typename Point, typename Map> class DiscreteGradientFlow
{
public:
    DiscreteGradientFlow(const LiftedGraphFunction<Point, Map> *initialFunction);

    void iterate();

    void getOutputFunction(LiftedGraphFunction<Point, Map> *outputFunction);
    double updateSupDelta();
    void reset();

    Point getValue(uint index) const {return newValues.at(index);}

private:
    void refreshNeighborsValuesKicked();
    void constantStepUpdateValues();
    void optimalStepUpdateValues();
    void refreshOutput();
    void computeGradient();

    std::vector<H2TangentVector> computeEnergyGradient(std::vector<H2Point> &values);
    double computeEnergyHessian(const std::vector<H2TangentVector> &V);


    void lineSearch();

    const uint nbBoundaryPoints;
    const uint nbPoints;
    const std::vector< std::vector<uint> > neighborsIndices;
    const std::vector< std::vector<double> > neighborsWeights;
    const std::vector< std::vector<Map> > boundaryPointsNeighborsPairingsValues;

    std::vector<Point> initialValues, oldValues, newValues;
    std::vector<H2TangentVector> gradient;
    std::vector< std::vector<Point> > oldNeighborsValuesKicked, newNeighborsValuesKicked;

    const std::unique_ptr<LiftedGraphFunction<Point, Map> > outputFunction;

    double constantStep, optimalStep;
    double supDelta;
    std::vector<double> errors;
};


#endif // DISCRETEGRADIENTFLOW_H
