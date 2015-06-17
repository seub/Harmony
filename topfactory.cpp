#include "topfactory.h"

#include "actionhandler.h"

TopFactory::TopFactory()
{    
}

void TopFactory::setHandler(ActionHandler *handler)
{
    this->handler = handler;
    connect(&(this->subfactory), SIGNAL(finished()), handler, SLOT(finishedComputing()));
}

void TopFactory::resetInitSubfactory()
{
    subfactory.resetInit();
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
    subfactory.iterate(N);
}

void TopFactory::runHeatFlow()
{
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
