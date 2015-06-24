#include "discreteheatflowiterator.h"

#include "liftedgraph.h"


template <typename Point, typename Map>
DiscreteHeatFlowIterator<Point, Map>::DiscreteHeatFlowIterator(const std::shared_ptr<LiftedGraphFunction<Point, Map> > &initialFunction) :
    nbBoundaryPoints(initialFunction->nbBoundaryPoints), nbPoints(initialFunction->nbPoints),
    neighborsIndices(initialFunction->neighborsIndices), neighborsWeights(initialFunction->neighborsWeights),
    boundaryPointsNeighborsPairingsValues(initialFunction->boundaryPointsNeighborsPairingsValues)
{
    initialValues = initialFunction->values;
    outputFunction = initialFunction->cloneCopyConstruct();

    reset();
}

template <typename Point, typename Map>
void DiscreteHeatFlowIterator<Point, Map>::reset()
{

    newValues = initialValues;

    newNeighborsValuesKicked.resize(nbPoints);
    uint i=0, j;
    while(i != nbBoundaryPoints)
    {
        j=0;
        for (auto neighborIndex : neighborsIndices[i])
        {
            newNeighborsValuesKicked[i].push_back(boundaryPointsNeighborsPairingsValues[i][j]*newValues[neighborIndex]);
            ++j;
        }
        ++i;
    }
    while (i != nbPoints)
    {
        for (auto neighborIndex : neighborsIndices[i])
        {
            newNeighborsValuesKicked[i].push_back(newValues[neighborIndex]);
        }
        ++i;
    }
}

template <typename Point, typename Map>
void DiscreteHeatFlowIterator<Point, Map>::getOutputFunction(const std::shared_ptr<LiftedGraphFunction<Point, Map> > &outputFunction)
{
    refreshOutput();
    outputFunction->cloneCopyAssign(this->outputFunction);
}

template <typename Point, typename Map>
void DiscreteHeatFlowIterator<Point, Map>::refreshOutput()
{
    outputFunction->resetValues(newValues);
}


template <typename Point, typename Map>
void DiscreteHeatFlowIterator<Point, Map>::refreshNeighborsValuesKicked()
{
    oldNeighborsValuesKicked = newNeighborsValuesKicked;
    uint i=0, j;
    while(i != nbBoundaryPoints)
    {
        j=0;
        for (auto neighborIndex : neighborsIndices[i])
        {
            newNeighborsValuesKicked[i][j] = boundaryPointsNeighborsPairingsValues[i][j]*newValues[neighborIndex];
            ++j;
        }
        ++i;
    }
    while (i != nbPoints)
    {
        j=0;
        for (auto neighborIndex : neighborsIndices[i])
        {
            newNeighborsValuesKicked[i][j] = newValues[neighborIndex];
            ++j;
        }
        ++i;
    }
}

template <typename Point, typename Map>
void DiscreteHeatFlowIterator<Point, Map>::updateValues()
{
    oldValues = newValues;

    for (uint i=0; i!=nbPoints; ++i)
    {
        newValues[i] = H2Point::centroid(newNeighborsValuesKicked[i], neighborsWeights[i]);
    }
}

template <typename Point, typename Map>
void DiscreteHeatFlowIterator<Point, Map>::iterate()
{
    refreshNeighborsValuesKicked();
    updateValues();
}


template <typename Point, typename Map>
void DiscreteHeatFlowIterator<Point, Map>::iterate(uint nbIterations)
{
    for (uint i=0; i!=nbIterations; ++i)
    {
        iterate();
    }
}







template class DiscreteHeatFlowIterator<H2Point, H2Isometry>;
