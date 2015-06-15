#include "actionhandler.h"
#include "equivariantharmonicmapsfactory.h"
#include "h2canvasdelegate.h"
#include "mathscontainer.h"
#include "window.h"
#include "canvas.h"

ActionHandler::ActionHandler()
{
}

void ActionHandler::setWindow(Window *window)
{
    this->window = window;
    leftDelegate = (H2CanvasDelegate *) window->leftCanvas->delegate;
    rightDelegate = (H2CanvasDelegate *) window->rightCanvas->delegate;
}

void ActionHandler::setContainer(MathsContainer *container)
{
    this->container = container;
    factory = &(this->container->factory);
    setFactory();
}

void ActionHandler::setFactory()
{
    int g = 2;
    std::vector<double> lengths1,lengths2;
    std::vector<double> twists1,twists2;
    for (int i=0; i<3*g-3; i++)
    {
        twists1.push_back(1.0-.3*i);
        //twists1.push_back(0.0);
        twists2.push_back(0.0);
    }
    twists2[2] = 2.0*M_PI;

    for (int i=0; i<3*g-3; i++)
    {
        lengths1.push_back(2);
        lengths2.push_back(1.0+3.0*i);
    }

    //EquivariantHarmonicMapsFactory F;
    factory->setGenus(2);
    //F.setRhoDomain(lengths1, twists1);
    factory->setNiceRhoDomain();
    factory->setRhoTarget(lengths2, twists2);
    //F.setNiceRhoTarget();

    factory->setMeshDepth(2);
    factory->initialize();


    leftDelegate->buffer.addElement(factory->rhoDomain, "blue", 2);
    leftDelegate->buffer.addElement(&factory->mesh, "red", 1);
    leftDelegate->buffer.addKickedDrawing(factory->rhoDomain.getSidePairingsNormalizedAroundVertex());
    leftDelegate->redrawBuffer();

    rightDelegate->buffer.addElement(factory->rhoTarget, "blue", 2);
    rightDelegate->buffer.addElement(&factory->functionInit, "red", 1);
    rightDelegate->buffer.setIsometries(factory->rhoTarget.getSidePairingsNormalizedAroundVertex());
    rightDelegate->buffer.addKickedDrawing(factory->rhoTarget.getSidePairingsNormalizedAroundVertex());

    rightDelegate->redrawBuffer();
}
