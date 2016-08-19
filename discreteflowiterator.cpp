#include "discreteflowiterator.h"
#include "liftedgraph.h"


template <typename Point, typename Map>
DiscreteFlowIterator<Point, Map>::DiscreteFlowIterator(const LiftedGraphFunction<Point, Map> *initialFunction) :
    nbBoundaryPoints(initialFunction->nbBoundaryPoints),
    nbPoints(initialFunction->nbPoints),
    neighborsIndices(initialFunction->neighborsIndices),
    neighborsWeightsCentroid(initialFunction->neighborsWeightsCentroid),
    neighborsWeightsEnergy(initialFunction->neighborsWeightsEnergy),
    boundaryPointsNeighborsPairingsValues(initialFunction->boundaryPointsNeighborsPairingsValues),
    initialValues(initialFunction->getValues()),
    outputFunction(initialFunction->cloneCopyConstruct())
{
    constantStep=0.01;
    reset();
}


template <typename Point, typename Map>
void DiscreteFlowIterator<Point, Map>::reset()
{

    newValues = initialValues;
    errors.resize(nbPoints);
    gradient.resize(this->nbPoints);
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
    std::cout << "supDelta = " << supDelta << std::endl;
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
void DiscreteFlowIterator<Point, Map>::updateValues()
{
    //updateValuesCentroid();
    updateValuesEnergyConstantStep();
}



template <typename Point, typename Map>
void DiscreteFlowIterator<Point, Map>::updateValuesCentroid()
{
    this->oldValues = this->newValues;

    for (uint i=0; i!=this->nbPoints; ++i)
    {
        this->newValues[i] = H2Point::centroid(this->neighborsValuesKicked[i], this->neighborsWeightsEnergy[i]);
    }

	this->refreshNeighborsValuesKicked();
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
void DiscreteFlowIterator<Point, Map>::updateValuesEnergyConstantStep()
{
    this->oldValues = this->newValues;

    computeGradient();
    this->newValues = H2TangentVector::exponentiate(-constantStep*gradient);
    this->refreshNeighborsValuesKicked();
}


template <typename Point, typename Map>
void DiscreteFlowIterator<Point, Map>::updateValuesEnergyOptimalStep()
{
    this->oldValues = this->newValues;

    computeGradient();
    lineSearch();
    this->newValues = H2TangentVector::exponentiate(-optimalStep*gradient);
    this->refreshNeighborsValuesKicked();
}


template <typename Point, typename Map>
void DiscreteFlowIterator<Point, Map>::lineSearch()
{
    std::vector<H2Point> yt;
    std::vector<H2TangentVector> dyt;
}


template <typename Point, typename Map>
void DiscreteFlowIterator<Point, Map>::computeGradient()
{
    H2TangentVector v;
    for (uint i=0; i!=this->nbPoints; ++i)
    {
        this->oldValues[i].weightedLogSum(this->neighborsValuesKicked[i], this->neighborsWeightsCentroid[i], v);
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
//template class DiscreteHeatFlowIteratorRecursiveDepth<H2Point, H2Isometry>;

