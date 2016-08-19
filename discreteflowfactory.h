#ifndef DISCRETEFLOWFACTORY_H
#define DISCRETEFLOWFACTORY_H

#include <QThread>

#include "tools.h"
#include "grouprepresentation.h"
#include "discreteflowiterator.h"
#include "liftedgraph.h"


template <typename Point, typename Map> class LiftedGraphFunctionTriangulated; class H2DiscreteFlowFactoryThread;

template <typename Point, typename Map> class DiscreteFlowFactory
{
    friend class ActionHandler;
    friend class H2DiscreteFlowFactoryThread;

public:
    DiscreteFlowFactory(GroupRepresentation<H2Isometry> *rhoDomain,
                            GroupRepresentation<H2Isometry> *rhoImage,
                            LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *domainFunction,
                            LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *imageFunction);
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

    double getSupError() const {return supError;}
    void updateSupError();
    double getTolerance() const;

    void setFlowChoice(int flowChoice);

    void run();
    void iterate(uint N);
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

    GroupRepresentation<H2Isometry> *rhoDomain;
    GroupRepresentation<Map> *rhoImage;
    LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *domainFunction;
    std::unique_ptr<LiftedGraphFunctionTriangulated<Point, Map> > initialImageFunction;
    LiftedGraphFunctionTriangulated<Point, Map> *imageFunction;
    std::unique_ptr<DiscreteFlowIterator<Point, Map> > iterator;
    uint nbIterations;
    double minDomainEdgeLength, supError, tolerance;

    int flowChoice;

    H2DiscreteFlowFactoryThread *thread;
};

#endif // DISCRETEFLOWFACTORY_H
