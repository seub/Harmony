#ifndef H2DISCRETEFLOWFACTORYTHREAD_H
#define H2DISCRETEFLOWFACTORYTHREAD_H

#include <QThread>

#include "tools.h"
#include "discreteheatflowfactory.h"

class H2DiscreteFlowFactoryThread : public QThread
{
    Q_OBJECT

public:
    H2DiscreteFlowFactoryThread(const std::shared_ptr<LiftedGraphFunctionTriangulated<H2Point, H2Isometry> > &domainFunction,
                                const std::shared_ptr<LiftedGraphFunctionTriangulated<H2Point, H2Isometry> > &imageFunction);

    uint getNbIterations() const;

public slots:
    void run();
    void stopRunning();

private:
    DiscreteHeatFlowFactory<H2Point, H2Isometry> factory;
};

#endif // H2DISCRETEFLOWFACTORYTHREAD_H
