#ifndef DISCRETEFLOWENERGY_H
#define DISCRETEFLOWENERGY_H

#include "tools.h"
#include "h2tangentvector.h"


template <typename Point, typename Map> class LiftedGraphFunction;
//class H2TangentVector;

template <typename Point, typename Map> class DiscreteFlowIteratorEnergy
{
public:
    DiscreteFlowIteratorEnergy(const LiftedGraphFunction<Point, Map> *initialFunction);

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

//    double computeEnergyHessian(const std::vector<H2TangentVector> &V);


    void lineSearch();

    const uint nbBoundaryPoints;
    const uint nbPoints;
    const std::vector< std::vector<uint> > neighborsIndices;
    const std::vector< std::vector<double> > neighborsWeights;
    const std::vector< std::vector<Map> > boundaryPointsNeighborsPairingsValues;

    std::vector<Point> initialValues, oldValues, newValues;
    std::vector<H2TangentVector> gradient;
    std::vector< std::vector<Point> > neighborsValuesKicked;

    const std::unique_ptr<LiftedGraphFunction<Point, Map> > outputFunction;

    double constantStep, optimalStep;
    double supDelta;
    std::vector<double> errors;
};


#endif // DISCRETEFLOWENERGY_H
