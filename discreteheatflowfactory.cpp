#include "discreteheatflowfactory.h"

#include "fenchelnielsenconstructor.h"
#include "h2discreteflowfactorythread.h"

template<typename Point, typename Map>
DiscreteHeatFlowFactory<Point, Map>::DiscreteHeatFlowFactory(GroupRepresentation<H2Isometry> *rhoDomain,
                                                             GroupRepresentation<H2Isometry> *rhoImage,
                                                             LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *domainFunction,
                                                             LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *imageFunction) :
    rhoDomain(rhoDomain), rhoImage(rhoImage), domainFunction(domainFunction), initialImageFunction(nullptr), imageFunction(imageFunction),
    iterator(nullptr)
{
    isGenusSet = false;
    isRhoDomainSet = false;
    isRhoImageSet = false;
    isMeshDepthSet = false;

    tolerance = 0.0000000001;
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::setGenus(uint genus)
{
    this->genus = genus;
    isGenusSet = true;
    isRhoDomainSet = false;
    isRhoImageSet = false;
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::setNiceRhoDomain()
{
    if (!isGenusSet)
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::setNiceRhoDomain(): genus not set"));
    }
    if (genus != 2)
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::setNiceRhoDomain(): genus needs to be 2 for 'nice' representation"));
    }
    rhoDomain->setNiceRepresentation();
    isRhoDomainSet = true;
    initializeRhoDomain();
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::initializeRhoDomain()
{

    if (!(isGenusSet && isRhoDomainSet))
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::initializeRhoDomain(): not ready to initialize rho domain"));
    }
    if (isMeshDepthSet)
    {
        initializeDomainFunction();
    }

}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::initializeRhoImage()
{
    if (!(isGenusSet && isRhoImageSet))
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::initializeRhoImage(): not ready to initialize rho image"));
    }

    if (isGenusSet && isMeshDepthSet && isRhoDomainSet)
    {
        initializeImageFunction();
    }
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::setNiceRhoImage()
{
    if (!(isGenusSet))
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::setNiceRhoImage(): genus not set"));
    }
    if (genus != 2)
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::setNiceRhoImage(): genus needs to be 2"));
    }
    rhoImage->setNiceRepresentation();

    isRhoImageSet = true;
    initializeRhoImage();
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::setMeshDepth(uint meshDepth)
{
    this->meshDepth = meshDepth;
    isMeshDepthSet = true;
    if (isGenusSet && isRhoDomainSet)
    {
        initializeDomainFunction();
    }
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::setRhoDomain(const std::vector<double> &FNLengths, const std::vector<double> FNTwists)
{


    if (!isGenusSet)
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::setRhoDomain(): genus has not been set"));
    }
    else if ((3*genus != (3+FNLengths.size())) || (3*genus != (3+FNTwists.size())))
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::setRhoDomain(): genus does not match number of FN coordinates"));
    }
    FNLengthsDomain = FNLengths;
    FNTwistsDomain = FNTwists;

    FenchelNielsenConstructor FN(FNLengths, FNTwists);
    *rhoDomain = FN.getRepresentation();
    isRhoDomainSet = true;

    initializeRhoDomain();
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::setRhoImage(const std::vector<double> &FNLengths, const std::vector<double> FNTwists)
{
    if (!isGenusSet)
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::setRhoImage(): genus has not been set"));
    }
    else if ((3*genus != (3+FNLengths.size())) || (3*genus != (3+FNTwists.size())))
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::setRhoImage(): genus does not match number of FN coordinates"));
    }
    FNLengthsImage = FNLengths;
    FNTwistsImage = FNTwists;
    FenchelNielsenConstructor FN(FNLengths, FNTwists);
    *rhoImage = FN.getRepresentation();

    isRhoImageSet = true;
    initializeRhoImage();
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::resetRhoDomain()
{
    isRhoDomainSet = false;
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::resetRhoImage()
{
    isRhoImageSet = false;
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::initializeDomainFunction()
{
    if (!(isGenusSet && isRhoDomainSet && isMeshDepthSet))
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::initializeDomainFunction(): not ready to initialize domain function"));
    }

    // I guess I should define a "clone move" in Lifted Graph for here
    LiftedGraphFunctionTriangulated<Point, Map> tempDomainFunction(*rhoDomain, meshDepth);
    domainFunction->cloneCopyAssign(&tempDomainFunction);

    minDomainEdgeLength = domainFunction->getMinEdgeLengthForRegularTriangulation();

    if (isRhoImageSet)
    {
        initializeImageFunction();
    }
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::initializeImageFunction()
{
    if (!isReady())
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::initializeImageFunction: Factory not ready to image function"));
    }

    initialImageFunction.reset(new LiftedGraphFunctionTriangulated<Point, Map>(*domainFunction, *rhoImage));
    resetInitial();
}

template<typename Point, typename Map>
bool DiscreteHeatFlowFactory<Point, Map>::isReady() const
{
    return isGenusSet && isRhoDomainSet && isRhoImageSet && isMeshDepthSet;
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::resetInitial()
{
    if (!isReady())
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::resetInit: Factory not ready to reset initial"));
    }
    imageFunction->cloneCopyAssign(initialImageFunction.get());
    iterator.reset(new DiscreteFlowIteratorCentroid<Point, Map>(initialImageFunction.get()));
    //iterator.reset(new DiscreteGradientFlow<Point, Map>(initialImageFunction.get()));
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::refreshImageFunction()
{
    iterator->getOutputFunction(imageFunction);
}

template <typename Point, typename Map>
bool DiscreteHeatFlowFactory<Point, Map>::isDomainFunctionInitialized(uint &nbMeshPointsOut)
{
    if (isGenusSet && isRhoDomainSet && isMeshDepthSet)
    {
        nbMeshPointsOut = domainFunction->getNbPoints();
        return true;
    }
    else
    {
        return false;
    }
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::updateSupError()
{
    supError = 2.0*iterator->updateSupDelta()/(minDomainEdgeLength*minDomainEdgeLength);
}

template<typename Point, typename Map>
double DiscreteHeatFlowFactory<Point, Map>::getTolerance() const
{
    return tolerance;
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::run()
{
    stop = false;
    nbIterations = 0;
    while(!stop)
    {
        iterator->iterate();
        ++nbIterations;

        if ((nbIterations % 8)==0)
        {
            updateSupError();
            if (supError < tolerance)
            {
                break;
            }
        }
    }
    updateSupError();
    refreshImageFunction();
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::iterate(uint N)
{
    stop = false;
    nbIterations = 0;
    while(!stop && nbIterations<N)
    {
        iterator->iterate();
        ++nbIterations;

        if ((nbIterations % 8)==0)
        {
            updateSupError();
            if (supError < tolerance)
            {
                break;
            }
        }
    }
    updateSupError();
    refreshImageFunction();
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::stopRunning()
{
    stop = true;
}


template class DiscreteHeatFlowFactory<H2Point, H2Isometry>;
