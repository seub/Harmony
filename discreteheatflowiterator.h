#ifndef DISCRETEHEATFLOWITERATOR_H
#define DISCRETEHEATFLOWITERATOR_H

#include "tools.h"

template <typename Point, typename Map> class LiftedGraphFunction;

template <typename Point, typename Map> class DiscreteHeatFlowIterator
{

public:
    DiscreteHeatFlowIterator(const LiftedGraphFunction<Point, Map> *initialFunction);

    void iterate();
    void iterate(uint nbIterations);
    void getOutputFunction(LiftedGraphFunction<Point, Map> *outputFunction);
    double updateSupDelta();
    void reset();

    Point getValue(uint index) const {return newValues.at(index);}

private:
    void refreshNeighborsValuesKicked();
    void updateValues();
    void refreshOutput();

    const uint nbBoundaryPoints;
    const uint nbPoints;
    const std::vector< std::vector<uint> > neighborsIndices;
    const std::vector< std::vector<double> > neighborsWeights;
    const std::vector< std::vector<Map> > boundaryPointsNeighborsPairingsValues;

    std::vector<Point> initialValues, oldValues, newValues;
    std::vector< std::vector<Point> > oldNeighborsValuesKicked, newNeighborsValuesKicked;

    const std::unique_ptr<LiftedGraphFunction<Point, Map> > outputFunction;

    double supDelta;
    std::vector<double> errors;
};

#endif // DISCRETEHEATFLOWITERATOR_H
