#include "topfactory.h"

#include "actionhandler.h"

TopFactory::TopFactory()
{    
}

void TopFactory::setHandler(ActionHandler *handler)
{
    this->handler = handler;
    connect(&(this->subfactory), SIGNAL(finished()), this, SLOT(finishedComputing()));
}

void TopFactory::finishedComputing()
{
    time = clock()-time;
    handler->finishedComputing();
}

void TopFactory::resetInitSubfactory()
{
    subfactory.resetInit();
}

double TopFactory::getTimeElapsed() const
{
    return time*1.0/CLOCKS_PER_SEC;
}

int TopFactory::getNbIterations() const
{
    return subfactory.nbIterations;
}

bool TopFactory::isMeshInitialized() const
{
    return subfactory.isMeshInitialized;
}

bool TopFactory::isFunctionInitialized() const
{
    return subfactory.isInitialized;
}

void TopFactory::iterateSubfactory(int N)
{
    time = clock();
    subfactory.iterate(N);
    time = clock()-time;
}

void TopFactory::runHeatFlow()
{
    time = clock();
    subfactory.stop = false;
    subfactory.start();
}

void TopFactory::stopHeatFlow()
{
    subfactory.stop = true;
}

void TopFactory::setGenus(int genus)
{
    subfactory.setGenus(genus);
}

void TopFactory::setMeshDepth(int meshDepth)
{
    subfactory.setMeshDepth(meshDepth);
}
