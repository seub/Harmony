#ifndef DISCRETEHEATFLOWITERATOR_H
#define DISCRETEHEATFLOWITERATOR_H

#include "tools.h"

template <typename Point, typename Map> class LiftedGraphFunction;

template <typename Point, typename Map> class DiscreteHeatFlowIterator
{

public:
    DiscreteHeatFlowIterator(const std::shared_ptr< LiftedGraphFunction<Point, Map> > &initialFunction);

    void iterate();
    void iterate(uint nbIterations);
    void getOutputFunction(const std::shared_ptr<LiftedGraphFunction<Point, Map> > &outputFunction);
    void reset();

private:
    void refreshNeighborsValuesKicked();
    void updateValues();
    void refreshOutput();

    std::shared_ptr<LiftedGraphFunction<Point, Map> > outputFunction;

    const uint nbBoundaryPoints;
    const uint nbPoints;
    const std::vector< std::vector<uint> > neighborsIndices;
    const std::vector< std::vector<double> > neighborsWeights;
    const std::vector< std::vector<Map> > boundaryPointsNeighborsPairingsValues;

    std::vector<Point> initialValues, oldValues, newValues;
    std::vector< std::vector<Point> > oldNeighborsValuesKicked, newNeighborsValuesKicked;
};

#endif // DISCRETEHEATFLOWITERATOR_H
