#include "actionhandler.h"
#include "equivariantharmonicmapsfactory.h"
#include "h2canvasdelegate.h"
#include "mathscontainer.h"
#include "window.h"
#include "canvas.h"
#include "outputmenu.h"

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
        twists1.push_back(0.0);
        //twists1.push_back(0.0);
        twists2.push_back(0.0);
    }
    twists2[1] = -2.0*M_PI;

    for (int i=0; i<3*g-3; i++)
    {
        lengths1.push_back(2.0);
        lengths2.push_back(2.0);
    }

    factory->setGenus(2);
    //factory->setNiceRhoDomain();
    factory->setRhoDomain(lengths1, twists1);
    //factory->setRhoTarget(lengths2, twists2);
    factory->setNiceRhoTarget();

    factory->setMeshDepth(3);


    factory->initialize();

    leftDelegate->buffer.addElement(factory->rhoDomain, "blue", 2);
    leftDelegate->buffer.addElement(&factory->mesh, "red", 1);
    leftDelegate->buffer.setTranslations(factory->rhoDomain.getSidePairingsNormalizedAroundVertices());
    leftDelegate->buffer.addPolygonTranslatesDomain();


    rightDelegate->buffer.addElement(factory->rhoTarget, "blue", 2);
    rightDelegate->buffer.addElement(&factory->function, "red", 1);
    rightDelegate->buffer.setTranslations(factory->rhoTarget.getSidePairingsNormalizedAroundVertices());

    rightDelegate->addPolygonAndMeshTranslates();

    //rightDelegate->buffer.addMeshTranslates();
}

void ActionHandler::processMessage(actionHandlerMessage message, int timeOut)
{
    bool isVertexHighlighted, isTriangleHighlighted;
    int meshIndexHighlighted, index1, index2, index3;
    bool update;

    switch(message)
    {
    case HIGHLIGHTED_LEFT:
        leftDelegate->getMeshTriangleIndicesHighlighted(isTriangleHighlighted, index1, index2, index3);
        leftDelegate->getMeshIndexHighlighted(isVertexHighlighted, meshIndexHighlighted);
        rightDelegate->decideHighlightingTriangle(isTriangleHighlighted, update, index1, index2, index3);
        rightDelegate->decideHighlightingMeshPoints(isVertexHighlighted, update, meshIndexHighlighted);
        leftDelegate->redrawBuffer(false, true);
        rightDelegate->redrawBuffer(false, true);
        window->leftCanvas->update();
        window->rightCanvas->update();
        break;

    case HIGHLIGHTED_RIGHT:
        rightDelegate->getMeshTriangleIndicesHighlighted(isTriangleHighlighted, index1, index2, index3);
        rightDelegate->getMeshIndexHighlighted(isVertexHighlighted, meshIndexHighlighted);
        leftDelegate->decideHighlightingTriangle(isTriangleHighlighted, update, index1, index2, index3);
        leftDelegate->decideHighlightingMeshPoints(isVertexHighlighted, update, meshIndexHighlighted);
        leftDelegate->redrawBuffer(false, true);
        rightDelegate->redrawBuffer(false, true);
        window->leftCanvas->update();
        window->rightCanvas->update();
        break;

    case HIGHLIGHTED_RESET:
        leftDelegate->resetHighlighted();
        rightDelegate->resetHighlighted();
        break;

    default:
        throw(QString("Error in ActionHandler::processMessage: message unknown"));
    }
}

void ActionHandler::computeButtonClicked()
{

}

void ActionHandler::outputResetButtonClicked()
{
    factory->resetInit();
    window->outputMenu->resetMenu();
    ((H2CanvasDelegateTarget *) rightDelegate)->refreshFunction();
    //rightDelegate->addMeshTranslates();
    rightDelegate->addPolygonAndMeshTranslates();
    rightDelegate->enableRedrawBuffer();
    window->rightCanvas->update();
}

void ActionHandler::iterateButtonClicked()
{
    iterateDiscreteFlow(50);
    ((H2CanvasDelegateTarget *) rightDelegate)->refreshFunction();
    //rightDelegate->addMeshTranslates();
    rightDelegate->addPolygonAndMeshTranslates();
    rightDelegate->enableRedrawBuffer();
    window->rightCanvas->update();
    window->outputMenu->enableReset();
}

void ActionHandler::iterateDiscreteFlow(int N)
{
    factory->iterate(N);
}

