#include "h2discreteflowfactorythread.h"
#include "discreteflowfactory.h"
#include "actionhandler.h"


H2DiscreteFlowFactoryThread::H2DiscreteFlowFactoryThread(GroupRepresentation<H2Isometry> *rhoDomain,
                                                         GroupRepresentation<H2Isometry> *rhoImage,
                                                         LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *domainFunction,
                                                         LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *imageFunction) :
    factory(rhoDomain, rhoImage, domainFunction, imageFunction)
{
    factory.thread = this;
}
void H2DiscreteFlowFactoryThread::run()
{
    factory.run();
}

void H2DiscreteFlowFactoryThread::setFlowChoice(int flowChoice)
{
    factory.setFlowChoice(flowChoice);
}

void H2DiscreteFlowFactoryThread::iterate(uint N)
{
    factory.iterate(N);
}

void H2DiscreteFlowFactoryThread::stopRunning()
{
    factory.stopRunning();
}

uint H2DiscreteFlowFactoryThread::getNbIterations() const
{
    return factory.getNbIterations();
}

double H2DiscreteFlowFactoryThread::getSupError() const
{
    return factory.getSupError();
}

void H2DiscreteFlowFactoryThread::updateSupError()
{
    factory.updateSupError();
}

double H2DiscreteFlowFactoryThread::getTolerance() const
{
    return factory.getTolerance();
}
