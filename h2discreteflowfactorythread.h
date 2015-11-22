#ifndef H2DISCRETEFLOWFACTORYTHREAD_H
#define H2DISCRETEFLOWFACTORYTHREAD_H

#include <QThread>

#include "tools.h"
#include "discreteheatflowfactory.h"

class ActionHandler;

class H2DiscreteFlowFactoryThread : public QThread
{
    Q_OBJECT

    friend class TopFactory;
    friend class ActionHandler;

public:
    H2DiscreteFlowFactoryThread(GroupRepresentation<H2Isometry> *rhoDomain, GroupRepresentation<H2Isometry> *rhoImage,
                                LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *domainFunction,
                                LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *imageFunction);

    uint getNbIterations() const;
    void updateSupError();
    double getSupError() const;
    double getTolerance() const;

public slots:
    void run();
    void iterate(uint N);
    void stopRunning();

private:
    DiscreteHeatFlowFactory<H2Point, H2Isometry> factory;
};

#endif // H2DISCRETEFLOWFACTORYTHREAD_H
