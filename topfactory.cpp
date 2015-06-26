#include "topfactory.h"

#include "actionhandler.h"
#include "mathscontainer.h"
#include "discreteheatflowfactory.h"

TopFactory::TopFactory(MathsContainer * const mathsContainer, ActionHandler * const handler) :
    handler(handler), h2factory(&(mathsContainer->rhoDomain), &(mathsContainer->H2RhoImage), &(mathsContainer->domainFunction), &(mathsContainer->H2ImageFunction))
{
    time = new QTime();
    timer = new QTimer();

    connect(&(this->h2factory), SIGNAL(finished()), this, SLOT(finishedComputing()));
}

TopFactory::~TopFactory()
{
    delete timer;
    delete time;
}

void TopFactory::startTimers()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(sendLiveMessageForStatusBar()));

    time->start();
    timer->start(30);
}

void TopFactory::stopTimers()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(sendLiveMessageForStatusBar()));
    timer->stop();
}

void TopFactory::sendLiveMessageForStatusBar()
{
    nbIterations = h2factory.getNbIterations();
    double error = h2factory.getSupError();
    QString message = QString("Iterating discrete heat flow (for %1 mesh vertices)...          Iterations :  %2 Error :  %3 (target: %4)%5 Time elapsed :  %6s")
            .arg(nbVertices)
            .arg(nbIterations, -13)
            .arg(error, 0, 'e', 2)
            .arg(h2factory.getTolerance(), 0, 'e', 2)
            .arg("", -13)
            .arg(time->elapsed()*0.001, 0, 'g');
    handler->showStatusBarMessage(message);
}

void TopFactory::sendFinishedMessageForStatusBar()
{
    double error = h2factory.getSupError();
    QString message = QString("Iterated discrete heat flow %1 times (for %2 mesh vertices). Final error: %3. Time elapsed: %4s.")
            .arg(nbIterations)
            .arg(nbVertices)
            .arg(error, 0, 'e', 2)
            .arg(time->elapsed()*0.001, 0, 'g');
    handler->showStatusBarMessage(message);
}

void TopFactory::finishedComputing()
{
    nbIterations = h2factory.getNbIterations();
    stopTimers();
    sendFinishedMessageForStatusBar();
    handler->processMessage(ActionHandlerMessage::FINISHED_COMPUTING);
}

void TopFactory::resetH2Flow()
{
    h2factory.factory.resetInitial();
}

void TopFactory::runH2Flow()
{
    startTimers();
    h2factory.start();
}

void TopFactory::stopH2Flow()
{
    h2factory.stopRunning();
}

void TopFactory::setGenus(uint genus)
{
    h2factory.factory.setGenus(genus);
}

void TopFactory::setMeshDepth(uint meshDepth)
{
    h2factory.factory.setMeshDepth(meshDepth);
    decideEmittingMeshCreated();
}

void TopFactory::setNiceRhoDomain()
{
    h2factory.factory.setNiceRhoDomain();
    decideEmittingMeshCreated();
}

void TopFactory::setNiceRhoImage()
{
    h2factory.factory.setNiceRhoImage();
}

void TopFactory::setRhoDomain(const std::vector<double> &FNlengths, const std::vector<double> FNtwists)
{
    h2factory.factory.setRhoDomain(FNlengths, FNtwists);
    decideEmittingMeshCreated();
}

void TopFactory::setRhoImage(const std::vector<double> &FNlengths, const std::vector<double> FNtwists)
{
    h2factory.factory.setRhoImage(FNlengths, FNtwists);
}


void TopFactory::decideEmittingMeshCreated()
{
    if (h2factory.factory.isDomainFunctionInitialized(nbVertices))
    {
        handler->meshCreated(nbVertices);
    }
}

void TopFactory::resetRhoDomain()
{
    h2factory.factory.resetRhoDomain();
}

void TopFactory::resetRhoImage()
{
    h2factory.factory.resetRhoImage();
}
