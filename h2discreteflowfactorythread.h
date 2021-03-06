#ifndef H2DISCRETEFLOWFACTORYTHREAD_H
#define H2DISCRETEFLOWFACTORYTHREAD_H

#include <QThread>

#include "tools.h"
#include "discreteflowfactory.h"

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
    void updateEnergyError();
    void updateErrors();
    double getEnergyError() const;
    double getSupError() const;
    double getTolerance() const;
    void setFlowChoice(int flowChoice);

public slots:
    void run();
    void iterate(uint N);
    void stopRunning();

private:
    DiscreteFlowFactory<H2Point, H2Isometry> factory;
};

#endif // H2DISCRETEFLOWFACTORYTHREAD_H
