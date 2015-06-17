#include "actionhandler.h"

#include <QCheckBox>
#include <QComboBox>

#include "equivariantharmonicmapsfactory.h"
#include "topfactory.h"
#include "h2canvasdelegate.h"
#include "mathscontainer.h"
#include "window.h"
#include "canvas.h"
#include "outputmenu.h"
#include "inputmenu.h"
#include "displaymenu.h"

ActionHandler::ActionHandler()
{
    isShowingLive = false;
    isRhoDomainSet = false;
    isRhoImageSet = false;

    showTranslatesAroundVertexLeft = false;
    showTranslatesAroundVertexRight = false;
    showTranslatesAroundVerticesLeft = false;
    showTranslatesAroundVerticesRight = false;
}

void ActionHandler::setWindow(Window *window)
{
    this->window = window;
    inputMenu = window->inputMenu;
    displayMenu = window->displayMenu;
    outputMenu = window->outputMenu;
    leftCanvas = window->leftCanvas;
    rightCanvas = window->rightCanvas;
}

void ActionHandler::setContainer(MathsContainer *container)
{
    this->container = container;
    setFactory();
    resetDelegatePointers();
}

void ActionHandler::resetDelegatePointers()
{
    leftDelegate = (H2CanvasDelegate*) leftCanvas->delegate;
    rightDelegate = (H2CanvasDelegate*) rightCanvas->delegate;

    leftDelegate->buffer.setRhoPointer(&topFactory->subfactory.rhoDomain, "blue");
    leftDelegate->buffer.setMeshPointer(&topFactory->subfactory.mesh, "red");

    rightDelegate->buffer.setRhoPointer(&topFactory->subfactory.rhoTarget, "blue");
    rightDelegate->buffer.setFunctionPointer(&topFactory->subfactory.function, "red");
}

void ActionHandler::setFactory()
{
    topFactory = &(this->container->topFactory);
    topFactory->setHandler(this);
    topFactory->setGenus(inputMenu->getGenus());
    topFactory->setMeshDepth(inputMenu->getMeshDepth());
    resetDelegatePointers();

}

void ActionHandler::setReadyToCompute()
{
    if (!isReadyToCompute())
    {
        throw(QString("Error in ActionHandler::setReadyToCompute: test failed"));
    }
}

void ActionHandler::processMessage(actionHandlerMessage message, int parameter)
{
    bool isVertexHighlighted, isTriangleHighlighted;
    int meshIndexHighlighted, index1, index2, index3;
    bool update;

    switch(message)
    {
    case HIGHLIGHTED_LEFT:
        leftDelegate->enableRedrawBuffer(false, true);
        leftCanvas->update();
        if (isReadyToCompute())
        {
            leftDelegate->getMeshTriangleIndicesHighlighted(isTriangleHighlighted, index1, index2, index3);
            leftDelegate->getMeshIndexHighlighted(isVertexHighlighted, meshIndexHighlighted);
            rightDelegate->decideHighlightingTriangle(isTriangleHighlighted, update, index1, index2, index3);
            rightDelegate->decideHighlightingMeshPoints(isVertexHighlighted, update, meshIndexHighlighted);
            rightDelegate->enableRedrawBuffer(false, true);
            rightCanvas->update();
        }
        break;

    case HIGHLIGHTED_RIGHT:
        rightDelegate->enableRedrawBuffer(false, true);
        rightCanvas->update();
        if (isReadyToCompute())
        {
            rightDelegate->getMeshTriangleIndicesHighlighted(isTriangleHighlighted, index1, index2, index3);
            rightDelegate->getMeshIndexHighlighted(isVertexHighlighted, meshIndexHighlighted);
            leftDelegate->decideHighlightingTriangle(isTriangleHighlighted, update, index1, index2, index3);
            leftDelegate->decideHighlightingMeshPoints(isVertexHighlighted, update, meshIndexHighlighted);
            leftDelegate->enableRedrawBuffer(false, true);
            leftCanvas->update();
        }
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
    displayMenu->setEnabled(false);
    outputMenu->disableAllButStop();
    outputMenu->switchComputeToStopButton();
    outputMenu->update();

    isShowingLive = outputMenu->showLiveCheckbox->checkState();
    topFactory->runHeatFlow();
    if (isShowingLive)
    {
        rightDelegate->getShowTranslates(showTranslatesAroundVertexRight, showTranslatesAroundVerticesRight);
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

void ActionHandler::imageShowTranslatesClicked(int choice)
{
    bool aroundVertexOld = showTranslatesAroundVertexRight, aroundVerticesOld = showTranslatesAroundVerticesRight;
    bool aroundVertexNew, aroundVerticesNew;

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
        showTranslatesAroundVertexRight = aroundVertexNew;
        showTranslatesAroundVerticesRight = aroundVerticesNew;
        rightDelegate->setShowTranslates(aroundVertexNew, aroundVerticesNew);
        updateFunction(true);
    }
}

void ActionHandler::domainShowTranslatesClicked(int choice)
{
    bool aroundVertexOld = showTranslatesAroundVertexLeft, aroundVerticesOld = showTranslatesAroundVerticesLeft;
    bool aroundVertexNew, aroundVerticesNew;

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
        showTranslatesAroundVertexLeft = aroundVertexNew;
        showTranslatesAroundVerticesLeft = aroundVerticesNew;
        leftDelegate->setShowTranslates(aroundVertexNew, aroundVerticesNew);
        updateMesh(true);
    }
}

void ActionHandler::setRhoDomainClicked(int choice)
{
    //std::cout << "Entering ActionHandler::setRhoDomainClicked" << std::endl;

    switch(choice)
    {
    case SET_RHO_CHOOSE:
        isRhoDomainSet = false;
        topFactory->subfactory.resetRhoDomain();
        break;

    case SET_RHO_NICE:
        if (inputMenu->getGenus() == 2)
        {
            isRhoDomainSet = true;
            topFactory->subfactory.setNiceRhoDomain();
        }
        else
        {
            throw(QString("Popup Warning message: nice rep only available for genus 2"));
            isRhoDomainSet = false;
            inputMenu->setRhoDomainComboBox->setCurrentIndex(SET_RHO_CHOOSE);
            topFactory->subfactory.resetRhoTarget();
        }
        break;

    case SET_RHO_RANDOM:
        break;

    case SET_RHO_FN:
        break;

    default:
        throw(QString("Error in ActionHandler::setRhoDomainClicked: not supposed to land here"));
    }

    dealRhosReady();
}

void ActionHandler::setRhoImageClicked(int choice)
{
    //std::cout << "Entering ActionHandler::setRhoImageClicked" << std::endl;

    switch(choice)
    {
    case SET_RHO_CHOOSE:
        isRhoImageSet = false;
        topFactory->subfactory.resetRhoTarget();
        break;

    case SET_RHO_NICE:
        if (inputMenu->getGenus() == 2)
        {
            isRhoImageSet = true;
            topFactory->subfactory.setNiceRhoTarget();
        }
        else
        {
            throw(QString("Popup Warning message: nice rep only available for genus 2"));
            isRhoImageSet = false;
            inputMenu->setRhoImageComboBox->setCurrentIndex(SET_RHO_CHOOSE);
            topFactory->subfactory.resetRhoTarget();
        }
        break;

    case SET_RHO_RANDOM:
        break;

    case SET_RHO_FN:
        break;

    case SET_RHO_DEHNTWIST:
        break;

    default:
        throw(QString("Error in ActionHandler::setRhoImageClicked: not supposed to land here"));
    }

    dealRhosReady();
}

void ActionHandler::dealRhosReady()
{
    if (isRhoDomainSet)
    {
        leftDelegate->setIsRhoEmpty(false);
        leftDelegate->buffer.refreshRho();
        leftDelegate->setIsMeshEmpty(false);
        leftDelegate->buffer.refreshMesh();
        leftCanvas->setEnabled(true);
        updateMesh(true);

        if (isRhoImageSet)
        {
            rightDelegate->setIsRhoEmpty(false);
            rightDelegate->buffer.refreshRho();
            rightDelegate->setIsFunctionEmpty(false);
            rightDelegate->buffer.refreshFunction();
            rightCanvas->setEnabled(true);

            updateFunction(true);
            setDisplayMenuReady(true, true);
            outputMenu->setEnabled(true);
            setReadyToCompute();
        }
        else
        {
            rightCanvas->setEnabled(false);
            rightDelegate->setIsRhoEmpty(true);
            rightDelegate->setIsFunctionEmpty(true);
            setDisplayMenuReady(true, false);
            outputMenu->setEnabled(false);
        }
    }
    else
    {
        leftCanvas->setEnabled(false);
        rightCanvas->setEnabled(isRhoImageSet);
        displayMenu->setEnabled(false);
        outputMenu->setEnabled(false);

        leftDelegate->setIsRhoEmpty(true);
        leftDelegate->setIsMeshEmpty(true);

        rightDelegate->setIsRhoEmpty(!isRhoImageSet);
        if (isRhoImageSet)
        {
            rightDelegate->buffer.refreshRho();
        }
        rightDelegate->setIsFunctionEmpty(true);
    }

    leftDelegate->resetHighlighted();
    rightDelegate->resetHighlighted();
    leftDelegate->enableRedrawBuffer();
    rightDelegate->enableRedrawBuffer();
    leftCanvas->update();
    rightCanvas->update();
}

void ActionHandler::setDisplayMenuReady(bool left, bool right)
{
    displayMenu->setReady(left, right);
}

bool ActionHandler::isReadyToCompute() const
{
    bool test1 = (isRhoDomainSet && isRhoImageSet);
    bool test2 = topFactory->isFunctionInitialized();
    bool test3 = !(leftDelegate->buffer.isMeshEmpty || rightDelegate->buffer.isFunctionEmpty);
    bool test4 = outputMenu->isEnabled();
    bool test5 = leftCanvas->isEnabled() && rightCanvas->isEnabled();

    bool test = test1 || test2 || test3 || test4 || test5;
    if (test != (test1 && test2 && test3 && test4 && test5))
    {
        throw(QString("Error in ActionHandler::isReadyToCompute: test failed"));
    }
    return test;
}

void ActionHandler::genusClicked(int choice)
{
    inputMenu->resetRhos();
    topFactory->setGenus(choice);

    isRhoDomainSet = false;
    isRhoImageSet = false;

    dealRhosReady();
}

void ActionHandler::meshDepthClicked(int choice)
{
    topFactory->setMeshDepth(choice);

    dealRhosReady();
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
    if (!topFactory->isMeshInitialized())
    {
        throw(QString("Error in ActionHandler::updateMesh: not supposed to happen"));
    }

    ((H2CanvasDelegateDomain *) leftDelegate)->refreshMesh();
    if (updateTranslates)
    {
        leftDelegate->refreshTranslates();
    }
    else
    {
        leftDelegate->refreshTranslates(false, false);
    }
    leftDelegate->enableRedrawBuffer(true, false);
    leftCanvas->update();
}


void ActionHandler::updateFunction(bool updateTranslates)
{
    if (!topFactory->isFunctionInitialized())
    {
        throw(QString("Error in ActionHandler::updateFunction: not supposed to happen"));
    }

    ((H2CanvasDelegateTarget *) rightDelegate)->refreshFunction();
    if (updateTranslates)
    {
        rightDelegate->refreshTranslates();
    }
    else
    {
        rightDelegate->refreshTranslates(false, false);
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
    displayMenu->setEnabled(true);
    outputMenu->switchStopToComputeButton();
    outputMenu->enableAll();

    rightDelegate->setShowTranslates(showTranslatesAroundVertexRight, showTranslatesAroundVerticesRight);
    updateFunction(true);
}
