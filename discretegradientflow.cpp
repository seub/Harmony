#include "discretegradientflow.h"
#include "h2tangentvector.h"
#include "liftedgraph.h"



template <typename Point, typename Map>
DiscreteGradientFlow<Point, Map>::DiscreteGradientFlow(const LiftedGraphFunction<Point, Map> *initialFunction) :
    nbBoundaryPoints(initialFunction->nbBoundaryPoints), nbPoints(initialFunction->nbPoints),
    neighborsIndices(initialFunction->neighborsIndices), neighborsWeights(initialFunction->neighborsWeights),
    boundaryPointsNeighborsPairingsValues(initialFunction->boundaryPointsNeighborsPairingsValues), initialValues(initialFunction->getValues()),
    outputFunction(initialFunction->cloneCopyConstruct())
{
    constantStep=0.01;
    reset();
}

template <typename Point, typename Map>
void DiscreteGradientFlow<Point, Map>::reset()
{

    newValues = initialValues;
    errors.resize(nbPoints);

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
void DiscreteGradientFlow<Point, Map>::getOutputFunction(LiftedGraphFunction<Point, Map> *outputFunction)
{
    refreshOutput();
    outputFunction->cloneCopyAssign(this->outputFunction.get());
}

template <typename Point, typename Map>
void DiscreteGradientFlow<Point, Map>::refreshOutput()
{
    outputFunction->resetValues(newValues);
}


template <typename Point, typename Map>
void DiscreteGradientFlow<Point, Map>::refreshNeighborsValuesKicked()
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
void DiscreteGradientFlow<Point, Map>::constantStepUpdateValues()
{
    oldValues = newValues;

    computeGradient();
    newValues = H2TangentVector::exponentiate(constantStep*gradient);
}


template <typename Point, typename Map>
void DiscreteGradientFlow<Point, Map>::optimalStepUpdateValues()
{
    oldValues = newValues;

    computeGradient();
    lineSearch();
    newValues = H2TangentVector::exponentiate(optimalStep*gradient);
}


template <typename Point, typename Map>
void DiscreteGradientFlow<Point, Map>::lineSearch()
{
    std::vector<H2Point> yt;
    std::vector<H2TangentVector> dyt;
}


template <typename Point, typename Map>
void DiscreteGradientFlow<Point, Map>::iterate()
{
    refreshNeighborsValuesKicked();
    constantStepUpdateValues();
}

template <typename Point, typename Map>
void DiscreteGradientFlow<Point, Map>::computeGradient()
{
    for (uint i=0; i!=nbPoints; ++i)
    {
        oldValues[i].weightedLogSum(newNeighborsValuesKicked[i], neighborsWeights[i], gradient[i]);
    }
}


template <typename Point, typename Map>
std::vector<H2TangentVector> DiscreteGradientFlow<Point, Map>::computeEnergyGradient(std::vector<H2Point> &values)
{

    assert(values.size() == nbPoints);
    std::vector<H2TangentVector> out;


    std::vector<std::vector<H2Point>> neighborsValuesKicked = newNeighborsValuesKicked;
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


    out.resize(nbPoints);
    for (uint i=0; i!=nbPoints; ++i)
    {
        values[i].weightedLogSum(neighborsValuesKicked[i], neighborsWeights[i], out[i]);
    }

    return out;
}



template <typename Point, typename Map>
double DiscreteGradientFlow<Point, Map>::computeEnergyHessian(const std::vector<H2TangentVector> &V)
{

    assert(V.size() == nbPoints);

    std::vector<H2Point> values;
    values.reserve(nbPoints);
    for (const auto &v : V)
    {
        values.push_back(v.getRoot());
    }


    std::vector<std::vector<H2Point>> neighborsValuesKicked = newNeighborsValuesKicked;
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




template <typename Point, typename Map>
double DiscreteGradientFlow<Point, Map>::updateSupDelta()
{
    for (uint i=0; i!=nbPoints; ++i)
    {
        errors[i] = Point::distance(oldValues[i], newValues[i]);
    }

    supDelta = *std::max_element(errors.begin(), errors.end());
    return supDelta;
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



template class DiscreteGradientFlow<H2Point, H2Isometry>;
