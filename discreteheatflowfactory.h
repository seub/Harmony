#ifndef DISCRETEHEATFLOWFACTORY_H
#define DISCRETEHEATFLOWFACTORY_H

#include <QThread>

#include "tools.h"
#include "grouprepresentation.h"
#include "discreteheatflowiterator.h"

template <typename Point, typename Map> class LiftedGraphFunctionTriangulated;

template <typename Point, typename Map> class DiscreteHeatFlowFactory
{

public:
    DiscreteHeatFlowFactory(const std::shared_ptr<LiftedGraphFunctionTriangulated<H2Point, H2Isometry> > &domainFunction,
                            const std::shared_ptr<LiftedGraphFunctionTriangulated<Point, Map> > &imageFunction);
    void setGenus(uint genus);
    void setMeshDepth(uint meshDepth);
    void setNiceRhoDomain();
    void setNiceRhoImage();
    void setRhoDomain(const std::vector<double> & FNlengths, const std::vector<double> FNtwists);
    void setRhoImage(const std::vector<double> & FNlengths, const std::vector<double> FNtwists);

    void resetRhoDomain();
    void resetRhoImage();

    void resetInitial();
    bool isDomainFunctionInitialized(uint &nbMeshPointsOut);

    GroupRepresentation<H2Isometry> getRhoDomain() const;
    GroupRepresentation<H2Isometry> getRhoImage() const;

    void iterate(uint nbIterations=1);

    void run();
    void stopRunning();
    uint getNbIterations() const {return nbIterations;}

private:
    bool isReady() const;
    void initializeDomainFunction();
    void initializeImageFunction();
    void initializeRhoDomain();
    void initializeRhoImage();
    void refreshImageFunction();

    uint genus, meshDepth;

    bool isGenusSet, isMeshDepthSet, isRhoDomainSet, isRhoImageSet;
    bool stop;

    std::vector<double> FNLengthsDomain, FNTwistsDomain, FNLengthsImage, FNTwistsImage;

    GroupRepresentation<H2Isometry> rhoDomain;
    GroupRepresentation<Map> rhoImage;

    const std::shared_ptr< LiftedGraphFunctionTriangulated<H2Point, H2Isometry> > domainFunction;
    const std::shared_ptr< LiftedGraphFunctionTriangulated<Point, Map> > initialImageFunction, imageFunction;
    std::shared_ptr<DiscreteHeatFlowIterator<Point, Map> > iterator;
    uint nbIterations;
};

#endif // DISCRETEHEATFLOWFACTORY_H
