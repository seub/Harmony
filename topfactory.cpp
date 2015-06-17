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

void TopFactory::iterateSubfactory(int N)
{
    subfactory.iterate(N);
}

void TopFactory::initializeSubfactory()
{
    subfactory.initialize();
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
