#include "discreteflowiterator.h"
#include "liftedgraph.h"


template <typename Point, typename Map>
DiscreteFlowIterator<Point, Map>::DiscreteFlowIterator(const LiftedGraphFunction<Point, Map> *initialFunction) :
    nbBoundaryPoints(initialFunction->nbBoundaryPoints), nbPoints(initialFunction->nbPoints),
    neighborsIndices(initialFunction->neighborsIndices), neighborsWeights(initialFunction->neighborsWeights),
    boundaryPointsNeighborsPairingsValues(initialFunction->boundaryPointsNeighborsPairingsValues), initialValues(initialFunction->getValues()),
    outputFunction(initialFunction->cloneCopyConstruct())
{
    reset();
}


template <typename Point, typename Map>
void DiscreteFlowIterator<Point, Map>::reset()
{

    newValues = initialValues;
    errors.resize(nbPoints);

    neighborsValuesKicked.resize(nbPoints);
    uint i=0;

    while (i != nbPoints)
    {
        neighborsValuesKicked[i].resize(neighborsIndices[i].size());
        ++i;
    }
    refreshNeighborsValuesKicked();

}

template <typename Point, typename Map>
void DiscreteFlowIterator<Point, Map>::getOutputFunction(LiftedGraphFunction<Point, Map> *outputFunction)
{
    refreshOutput();
    outputFunction->cloneCopyAssign(this->outputFunction.get());
}

template <typename Point, typename Map>
double DiscreteFlowIterator<Point, Map>::updateSupDelta()
{
    for (uint i=0; i!=nbPoints; ++i)
    {
        errors[i] = Point::distance(oldValues[i], newValues[i]);
    }

    supDelta = *std::max_element(errors.begin(), errors.end());
    return supDelta;
}

template <typename Point, typename Map>
void DiscreteFlowIterator<Point, Map>::iterate()
{
    updateValues();
}



template <typename Point, typename Map>
void DiscreteFlowIterator<Point, Map>::iterate(uint nbIterations)
{
    for (uint i=0; i!=nbIterations; ++i)
    {
        iterate();
    }
}

template <typename Point, typename Map>
void DiscreteFlowIterator<Point, Map>::refreshOutput()
{
    outputFunction->resetValues(newValues);
}

template <typename Point, typename Map>
void DiscreteFlowIterator<Point, Map>::refreshNeighborsValuesKicked()
{
    uint i=0, j;
    while(i != nbBoundaryPoints)
    {
        j=0;
        for (auto neighborIndex : neighborsIndices[i])
        {
            neighborsValuesKicked[i][j] = boundaryPointsNeighborsPairingsValues[i][j]*newValues[neighborIndex];
            ++j;
        }
        ++i;
    }
    while (i != nbPoints)
    {
        j=0;
        for (auto neighborIndex : neighborsIndices[i])
        {
            neighborsValuesKicked[i][j] = newValues[neighborIndex];
            ++j;
        }
        ++i;
    }
}



template <typename Point, typename Map>
DiscreteFlowIteratorCentroid<Point, Map>::DiscreteFlowIteratorCentroid(const LiftedGraphFunction<Point, Map> *initialFunction) :
    DiscreteFlowIterator<Point,Map>(initialFunction)
{
}







template <typename Point, typename Map>
void DiscreteFlowIteratorCentroid<Point, Map>::updateValues()
{
    oldValues = newValues;

    for (uint i=0; i!=nbPoints; ++i)
    {
        newValues[i] = H2Point::centroid(neighborsValuesKicked[i], neighborsWeights[i]);
    }
}







/*
template <typename Map, typename Point>
DiscreteHeatFlowIteratorRecursiveDepth<Point, Map>::DiscreteHeatFlowIteratorRecursiveDepth(const GroupRepresentation<Map> &rhoDomain, const GroupRepresentation<Map> &rhoImage, uint depth) :
    rhoDomain(rhoDomain), rhoImage(rhoImage), depth(depth)
{
}

template <typename Map, typename Point>
bool DiscreteHeatFlowIteratorRecursiveDepth<Point, Map>::run()
{
    return (!stop);
}*/





template <typename Point, typename Map>
DiscreteFlowIteratorEnergy<Point, Map>::DiscreteFlowIteratorEnergy(const LiftedGraphFunction<Point, Map> *initialFunction) :
    DiscreteFlowIterator<Point,Map>(initialFunction)
{
    constantStep=0.01;
}


template <typename Point, typename Map>
void DiscreteFlowIteratorEnergy<Point, Map>::reset()
{
    DiscreteFlowIterator<Point,Map>::reset();
    gradient.resize(nbPoints);
}

template <typename Point, typename Map>
void DiscreteFlowIteratorEnergy<Point, Map>::updateValues()
{
    constantStepUpdateValues();
}




template <typename Point, typename Map>
void DiscreteFlowIteratorEnergy<Point, Map>::constantStepUpdateValues()
{
    oldValues = newValues;

    computeGradient();
    newValues = H2TangentVector::exponentiate(-constantStep*gradient);
    refreshNeighborsValuesKicked();
}


template <typename Point, typename Map>
void DiscreteFlowIteratorEnergy<Point, Map>::optimalStepUpdateValues()
{
    oldValues = newValues;

    computeGradient();
    lineSearch();
    newValues = H2TangentVector::exponentiate(-optimalStep*gradient);
    refreshNeighborsValuesKicked();
}


template <typename Point, typename Map>
void DiscreteFlowIteratorEnergy<Point, Map>::lineSearch()
{
    std::vector<H2Point> yt;
    std::vector<H2TangentVector> dyt;
}


template <typename Point, typename Map>
void DiscreteFlowIteratorEnergy<Point, Map>::computeGradient()
{
    H2TangentVector v;
    for (uint i=0; i!=nbPoints; ++i)
    {
        oldValues[i].weightedLogSum(neighborsValuesKicked[i], neighborsWeights[i], v);
        gradient[i]=-1.0*v;
    }
}


/*
template <typename Point, typename Map>
double DiscreteFlowIteratorEnergy<Point, Map>::computeEnergyHessian(const std::vector<H2TangentVector> &V)
{

    assert(V.size() == nbPoints);

    std::vector<H2Point> values;
    values.reserve(nbPoints);
    for (const auto &v : V)
    {
        values.push_back(v.getRoot());
    }


    std::vector<std::vector<H2Point>> neighborsValuesKicked = neighborsValuesKicked;
    // Gros porc

    uint i=0, j;
    while(i != nbBoundaryPoints)
    {
        j=0;
        for (auto neighborIndex : neighborsIndices[i])
        {
            neighborsValuesKicked[i][j] = boundaryPointsNeighborsPairingsValues[i][j]*values[neighborIndex];
            ++j;
        }
        ++i;
    }
    while (i != nbPoints)
    {
        j=0;
        for (auto neighborIndex : neighborsIndices[i])
        {
            neighborsValuesKicked[i][j] = values[neighborIndex];
            ++j;
        }
        ++i;
    }

    H2Point xv, xw;
    H2TangentVector uv, xvxw;
    double d, D, out = 0;
    uint v, w;


    for (v = 0; v!=nbPoints; ++v)
    {
        xv = values[v];
        uv = V[v];
        for (w=0; w!= neighborsIndices[v].size(); ++w)
        {
            xw = neighborsValuesKicked[v][w];
            xvxw = H2TangentVector(xv, xw);
            d = H2Point::distance(xv, xw);
            D = d/tanh(d);
            out += neighborsWeights[v][w]*(D*uv.lengthSquared() + (1-D)*(H2TangentVector::scalProd(uv, xvxw))/(d*d));
        }
    }


    return out;
}
*/








/*
template <typename Map, typename Point>
DiscreteHeatFlowIteratorRecursiveDepth<Point, Map>::DiscreteHeatFlowIteratorRecursiveDepth(const GroupRepresentation<Map> &rhoDomain, const GroupRepresentation<Map> &rhoImage, uint depth) :
    rhoDomain(rhoDomain), rhoImage(rhoImage), depth(depth)
{
}

template <typename Map, typename Point>
bool DiscreteHeatFlowIteratorRecursiveDepth<Point, Map>::run()
{
    return (!stop);
}*/




template class DiscreteFlowIterator<H2Point, H2Isometry>;
template class DiscreteFlowIteratorCentroid<H2Point, H2Isometry>;
//template class DiscreteHeatFlowIteratorRecursiveDepth<H2Point, H2Isometry>;
template class DiscreteFlowIteratorEnergy<H2Point, H2Isometry>;

