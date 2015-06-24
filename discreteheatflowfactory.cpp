#include "discreteheatflowfactory.h"


#include "fenchelnielsenconstructor.h"
#include "liftedgraph.h"


template<typename Point, typename Map>
DiscreteHeatFlowFactory<Point, Map>::DiscreteHeatFlowFactory(const std::shared_ptr< LiftedGraphFunctionTriangulated<H2Point, H2Isometry> > &domainFunction,
                                                             const std::shared_ptr< LiftedGraphFunctionTriangulated<Point, Map> > &imageFunction) :
    domainFunction(domainFunction), initialImageFunction(nullptr), imageFunction(imageFunction),
    iterator(nullptr)
{
    isGenusSet = false;
    isRhoDomainSet = false;
    isRhoImageSet = false;
    isMeshDepthSet = false;
    stop = false;
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
    rhoDomain.setNiceRepresentation();
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
    rhoImage.setNiceRepresentation();

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
    rhoDomain = FN.getRepresentation();
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
    rhoImage = FN.getRepresentation();

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
GroupRepresentation<H2Isometry> DiscreteHeatFlowFactory<Point, Map>::getRhoDomain() const
{
    return rhoDomain;
}

template<typename Point, typename Map>
GroupRepresentation<H2Isometry> DiscreteHeatFlowFactory<Point, Map>::getRhoImage() const
{
    return rhoImage;
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::initializeDomainFunction()
{
    if (!(isGenusSet && isRhoDomainSet && isMeshDepthSet))
    {
        throw(QString("Error in DiscreteHeatFlowFactory<Point, Map>::initializeDomainFunction(): not ready to initialize domain function"));
    }

    domainFunction->cloneCopyAssign(std::make_shared< LiftedGraphFunctionTriangulated<H2Point, H2Isometry> >(rhoDomain, meshDepth));

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

    initialImageFunction->cloneCopyAssign(std::make_shared< LiftedGraphFunctionTriangulated<Point, Map> >(*domainFunction, rhoImage));
    imageFunction->cloneCopyAssign(initialImageFunction);
    iterator = std::make_shared< DiscreteHeatFlowIterator<Point, Map> >(initialImageFunction);
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
    imageFunction->cloneCopyAssign(initialImageFunction);
    iterator = std::make_shared< DiscreteHeatFlowIterator<Point, Map> >(initialImageFunction);
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
void DiscreteHeatFlowFactory<Point, Map>::iterate(uint nbIterations)
{
    if (!isReady())
    {
        throw(QString("Error in void DiscreteHeatFlowFactory<Point, Map>::iterate(): not ready"));
    }
    iterator->iterate(nbIterations);
    refreshImageFunction();
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::run()
{
    nbIterations = 0;
    while(!stop)
    {
        iterator->iterate();
        ++nbIterations;
    }
    refreshImageFunction();
}

template<typename Point, typename Map>
void DiscreteHeatFlowFactory<Point, Map>::stopRunning()
{
    stop = true;
}


template class DiscreteHeatFlowFactory<H2Point, H2Isometry>;
