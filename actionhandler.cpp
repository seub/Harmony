#include "actionhandler.h"

#include <QCheckBox>

#include "equivariantharmonicmapsfactory.h"
#include "topfactory.h"
#include "h2canvasdelegate.h"
#include "mathscontainer.h"
#include "window.h"
#include "canvas.h"
#include "outputmenu.h"
#include "inputmenu.h"

ActionHandler::ActionHandler()
{
    isShowingLive = false;
}

void ActionHandler::setWindow(Window *window)
{
    this->window = window;
    inputMenu = window->inputMenu;
    outputMenu = window->outputMenu;
    leftCanvas = window->leftCanvas;
    rightCanvas = window->rightCanvas;

    leftDelegate = (H2CanvasDelegate *) window->leftCanvas->delegate;
    rightDelegate = (H2CanvasDelegate *) window->rightCanvas->delegate;
}

void ActionHandler::setContainer(MathsContainer *container)
{
    this->container = container;
    setFactory();
}

void ActionHandler::setFactory()
{
    topFactory = &(this->container->topFactory);
    topFactory->setHandler(this);

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

    topFactory->subfactory.setGenus(2);
    //factory->setNiceRhoDomain();
    topFactory->subfactory.setRhoDomain(lengths1, twists1);
    //factory->setRhoTarget(lengths2, twists2);
    topFactory->subfactory.setNiceRhoTarget();

    topFactory->subfactory.setMeshDepth(3);

    topFactory->initializeSubfactory();

    leftDelegate->buffer.addElement(topFactory->subfactory.rhoDomain, "blue", 2);
    leftDelegate->buffer.addElement(&topFactory->subfactory.mesh, "red", 1);
    leftDelegate->buffer.setTranslations();
    leftDelegate->addMeshTranslates();

    rightDelegate->buffer.addElement(topFactory->subfactory.rhoTarget, "blue", 2);
    rightDelegate->buffer.addElement(&topFactory->subfactory.function, "red", 1);
    rightDelegate->buffer.setTranslations();
    rightDelegate->addMeshTranslates();
}

void ActionHandler::processMessage(actionHandlerMessage message, int parameter)
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
        leftDelegate->enableRedrawBuffer(false, true);
        rightDelegate->enableRedrawBuffer(false, true);
        leftCanvas->update();
        rightCanvas->update();
        break;

    case HIGHLIGHTED_RIGHT:
        rightDelegate->getMeshTriangleIndicesHighlighted(isTriangleHighlighted, index1, index2, index3);
        rightDelegate->getMeshIndexHighlighted(isVertexHighlighted, meshIndexHighlighted);
        leftDelegate->decideHighlightingTriangle(isTriangleHighlighted, update, index1, index2, index3);
        leftDelegate->decideHighlightingMeshPoints(isVertexHighlighted, update, meshIndexHighlighted);
        leftDelegate->enableRedrawBuffer(false, true);
        rightDelegate->enableRedrawBuffer(false, true);
        leftCanvas->update();
        rightCanvas->update();
        break;

    case HIGHLIGHTED_RESET:
        leftDelegate->resetHighlighted();
        rightDelegate->resetHighlighted();
        break;

    case END_CANVAS_REPAINT:
        if (isShowingLive && parameter == H2DELEGATETARGET)
        {
            topFactory->subfactory.refreshFunction();
            updateFunction(false);
            rightDelegate->enableRedrawBuffer(true, false);
            rightCanvas->update();
        }
        break;

    default:
        throw(QString("Error in ActionHandler::processMessage: message unknown"));
    }
}

void ActionHandler::computeButtonClicked()
{
    leftCanvas->setEnabled(false);
    rightCanvas->setEnabled(false);
    inputMenu->setEnabled(false);
    outputMenu->disableAllButStop();
    outputMenu->switchComputeToStopButton();
    outputMenu->update();

    isShowingLive = outputMenu->showLiveCheckbox->checkState();
    topFactory->runHeatFlow();
    if (isShowingLive)
    {
        rightDelegate->getShowTranslates(showTranslatesAroundVertexOld, showTranslatesAroundVerticesOld);
        rightDelegate->setShowTranslates(false, false);
        rightDelegate->enableRedrawBuffer();
        rightCanvas->update();
    }
}

void ActionHandler::outputResetButtonClicked()
{
    topFactory->resetInitSubfactory();
    outputMenu->resetMenu();
    updateFunction(true);
}

void ActionHandler::iterateButtonClicked()
{
    iterateDiscreteFlow(outputMenu->getNbIterations());
    updateFunction(true);
    outputMenu->enableReset();
}

void ActionHandler::outputShowTranslatesChoice(int choice)
{
    bool aroundVertexOld, aroundVerticesOld, aroundVertexNew, aroundVerticesNew;
    rightDelegate->getShowTranslates(aroundVertexOld, aroundVerticesOld);

    switch(choice)
    {
    case SHOW_TRANSLATES_CHOOSE:
        aroundVertexNew = aroundVertexOld;
        aroundVerticesNew = aroundVerticesOld;
        break;

    case SHOW_TRANSLATES_DOMAIN:
        aroundVertexNew = false;
        aroundVerticesNew = false;
        break;

    case SHOW_TRANSLATES_VERTEX:
        aroundVertexNew = true;
        aroundVerticesNew = false;
        break;

    case SHOW_TRANSLATES_VERTICES:
        aroundVertexNew = false;
        aroundVerticesNew = true;
        break;

    default:
        throw(QString("Error in ActionHandler::outputShowTranslatesChoice: not supposed to land here"));
    }

    if ((aroundVertexNew != aroundVertexOld) || (aroundVerticesNew != aroundVerticesOld))
    {
        rightDelegate->setShowTranslates(aroundVertexNew, aroundVerticesNew);
        updateFunction(true);
    }
}

void ActionHandler::inputShowTranslatesChoice(int choice)
{
    bool aroundVertexOld, aroundVerticesOld, aroundVertexNew, aroundVerticesNew;
    leftDelegate->getShowTranslates(aroundVertexOld, aroundVerticesOld);

    switch(choice)
    {
    case SHOW_TRANSLATES_CHOOSE:
        aroundVertexNew = aroundVertexOld;
        aroundVerticesNew = aroundVerticesOld;
        break;

    case SHOW_TRANSLATES_DOMAIN:
        aroundVertexNew = false;
        aroundVerticesNew = false;
        break;

    case SHOW_TRANSLATES_VERTEX:
        aroundVertexNew = true;
        aroundVerticesNew = false;
        break;

    case SHOW_TRANSLATES_VERTICES:
        aroundVertexNew = false;
        aroundVerticesNew = true;
        break;

    default:
        throw(QString("Error in ActionHandler::outputShowTranslatesChoice: not supposed to land here"));
    }

    if ((aroundVertexNew != aroundVertexOld) || (aroundVerticesNew != aroundVerticesOld))
    {
        leftDelegate->setShowTranslates(aroundVertexNew, aroundVerticesNew);
        updateMesh(true);
    }
}


void ActionHandler::iterateDiscreteFlow(int N)
{
    topFactory->iterateSubfactory(N);
}

void ActionHandler::stopButtonClicked()
{
    topFactory->stopHeatFlow();
}

void ActionHandler::updateMesh(bool updateTranslates)
{
    ((H2CanvasDelegateDomain *) leftDelegate)->refreshMesh();
    if (updateTranslates)
    {
        leftDelegate->addMeshTranslates();
    }
    else
    {
        leftDelegate->addMeshTranslates(false, false);
    }
    leftDelegate->enableRedrawBuffer(true, false);
    leftCanvas->update();
}


void ActionHandler::updateFunction(bool updateTranslates)
{
    ((H2CanvasDelegateTarget *) rightDelegate)->refreshFunction();
    if (updateTranslates)
    {
        rightDelegate->addMeshTranslates();
    }
    else
    {
        rightDelegate->addMeshTranslates(false, false);
    }
    rightDelegate->enableRedrawBuffer(true, false);
    rightCanvas->update();
}

void ActionHandler::finishedComputing()
{
    isShowingLive = false;
    leftCanvas->setEnabled(true);
    rightCanvas->setEnabled(true);
    inputMenu->setEnabled(true);
    outputMenu->switchStopToComputeButton();
    outputMenu->enableAll();

    rightDelegate->setShowTranslates(showTranslatesAroundVertexOld, showTranslatesAroundVerticesOld);
    updateFunction(true);
}
