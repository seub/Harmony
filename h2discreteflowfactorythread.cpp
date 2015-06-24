#include "h2discreteflowfactorythread.h"



H2DiscreteFlowFactoryThread::H2DiscreteFlowFactoryThread(const std::shared_ptr<LiftedGraphFunctionTriangulated<H2Point, H2Isometry> > &domainFunction,
                                                         const std::shared_ptr<LiftedGraphFunctionTriangulated<H2Point, H2Isometry> > &imageFunction) :
    factory(domainFunction, imageFunction)
{
}


void H2DiscreteFlowFactoryThread::run()
{
    factory.run();
}

void H2DiscreteFlowFactoryThread::stopRunning()
{
    factory.stopRunning();
}

uint H2DiscreteFlowFactoryThread::getNbIterations() const
{
    return factory.getNbIterations();
}
