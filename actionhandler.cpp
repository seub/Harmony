#include "actionhandler.h"

#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>

#include "equivariantharmonicmapsfactory.h"
#include "topfactory.h"
#include "h2canvasdelegate.h"
#include "mathscontainer.h"
#include "window.h"
#include "canvas.h"
#include "outputmenu.h"
#include "inputmenu.h"
#include "displaymenu.h"
#include "fenchelnielsenuser.h"

ActionHandler::ActionHandler()
{
    isShowingLive = false;
    isRhoDomainSet = false;
    isRhoImageSet = false;

    showTranslatesAroundVertexLeft = false;
    showTranslatesAroundVertexRight = false;
    showTranslatesAroundVerticesLeft = false;
    showTranslatesAroundVerticesRight = false;

    expectingFNdomain = false;
    expectingFNtarget = false;
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

void ActionHandler::showTranslatesClicked(int choice)
{
    bool aroundVertexOldLeft = showTranslatesAroundVertexLeft, aroundVerticesOldLeft = showTranslatesAroundVerticesLeft;
    bool aroundVertexOldRight = showTranslatesAroundVertexRight, aroundVerticesOldRight = showTranslatesAroundVerticesRight;
    bool aroundVertexNewLeft, aroundVerticesNewLeft, aroundVertexNewRight, aroundVerticesNewRight;

    switch(choice)
    {
    case SHOW_TRANSLATES_DOMAIN:
        aroundVertexNewLeft = false;
        aroundVerticesNewLeft = false;
        aroundVertexNewRight = false;
        aroundVerticesNewRight = false;
        break;

    case SHOW_TRANSLATES_VERTEX:
        aroundVertexNewLeft = true;
        aroundVerticesNewLeft = false;
        aroundVertexNewRight = true;
        aroundVerticesNewRight = false;
        break;

    case SHOW_TRANSLATES_VERTICES:
        aroundVertexNewLeft = false;
        aroundVerticesNewLeft = true;
        aroundVertexNewRight = false;
        aroundVerticesNewRight = true;
        break;

    default:
        throw(QString("Error in ActionHandler::showTranslatesChoice: not supposed to land here"));
    }

    if ((aroundVertexNewLeft != aroundVertexOldLeft) || (aroundVerticesNewLeft != aroundVerticesOldLeft))
    {
        showTranslatesAroundVertexLeft = aroundVertexNewLeft;
        showTranslatesAroundVerticesLeft = aroundVerticesNewLeft;
        leftDelegate->setShowTranslates(aroundVertexNewLeft, aroundVerticesNewLeft);
        if (isRhoDomainSet)
        {
            updateMesh(true);
        }
    }

    if ((aroundVertexNewRight != aroundVertexOldRight) || (aroundVerticesNewRight != aroundVerticesOldRight))
    {
        showTranslatesAroundVertexRight = aroundVertexNewRight;
        showTranslatesAroundVerticesRight = aroundVerticesNewRight;
        rightDelegate->setShowTranslates(aroundVertexNewRight, aroundVerticesNewRight);
        if (isReadyToCompute())
        {
            updateFunction(true);
        }
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
        setRhoNiceDomain();
        break;

    case SET_RHO_RANDOM:
        setRhoRandomDomain();
        break;

    case SET_RHO_FN:
        setRhoFNDomain();
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
        dealRhosReady();
        break;

    case SET_RHO_NICE:
        setRhoNiceTarget();
        dealRhosReady();
        break;

    case SET_RHO_RANDOM:
        setRhoRandomTarget();
        dealRhosReady();
        break;

    case SET_RHO_FN:
        setRhoFNTarget();
        break;

    default:
        throw(QString("Error in ActionHandler::setRhoImageClicked: not supposed to land here"));
    }
}

void ActionHandler::setRhoNiceDomain()
{
    if (inputMenu->getGenus() == 2)
    {
        isRhoDomainSet = true;
        topFactory->subfactory.setNiceRhoDomain();
    }
    else
    {
        errorMessageForSetRhoNice();
        isRhoDomainSet = false;
        inputMenu->setRhoDomainComboBox->setCurrentIndex(SET_RHO_CHOOSE);
        topFactory->subfactory.resetRhoTarget();
    }
}

void ActionHandler::setRhoNiceTarget()
{
    if (inputMenu->getGenus() == 2)
    {
        isRhoImageSet = true;
        topFactory->subfactory.setNiceRhoTarget();
    }
    else
    {
        errorMessageForSetRhoNice();
        isRhoImageSet = false;
        inputMenu->setRhoImageComboBox->setCurrentIndex(SET_RHO_CHOOSE);
        topFactory->subfactory.resetRhoTarget();
    }
}

void ActionHandler::errorMessageForSetRhoNice()
{
    QMessageBox messageBox;
    QString message = QString("%1Nice%2 representation can only be set for genus 2").arg(QChar(8220)).arg(QChar(8221));


    QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setInformativeText(message);

    QGridLayout* layout = (QGridLayout*)messageBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    messageBox.exec();

}


void ActionHandler::setRhoRandomDomain()
{
    std::vector<double> lengths, twists;
    int genus = inputMenu->getGenus();
    ActionHandler::randomFNcoordinates(genus, lengths, twists);
    isRhoDomainSet = true;
    topFactory->subfactory.setRhoDomain(lengths, twists);
}

void ActionHandler::setRhoRandomTarget()
{
    std::vector<double> lengths, twists;
    int genus = inputMenu->getGenus();
    ActionHandler::randomFNcoordinates(genus, lengths, twists);
    isRhoImageSet = true;
    topFactory->subfactory.setRhoTarget(lengths, twists);
}

void ActionHandler::setRhoFNDomain()
{
    expectingFNdomain = true;
    window->setEnabled(false);
    FenchelNielsenUser *FNuser = new FenchelNielsenUser(this, inputMenu->getGenus());
    FNuser->show();
}

void ActionHandler::setRhoFNTarget()
{
    expectingFNtarget = true;
    window->setEnabled(false);
    FenchelNielsenUser *FNuser = new FenchelNielsenUser(this, inputMenu->getGenus());
    FNuser->show();
}

void ActionHandler::receiveFNcoordinates(const std::vector<double> &lengths, const std::vector<double> &twists)
{
    if (expectingFNdomain == expectingFNtarget)
    {
        throw(QString("Error in ActionHandler::receiveFNcoordinates: was expecting coordinates for neither/both domain and target"));
    }
    if (expectingFNdomain)
    {
        isRhoDomainSet = true;
        topFactory->subfactory.setRhoDomain(lengths, twists);
        expectingFNdomain = false;
    }
    if (expectingFNtarget)
    {
        isRhoImageSet = true;
        topFactory->subfactory.setRhoTarget(lengths, twists);
        expectingFNtarget = false;
    }
    window->setEnabled(true);
    dealRhosReady();
}

void ActionHandler::discardReceiveFNcoordinates()
{
    if (expectingFNdomain == expectingFNtarget)
    {
        throw(QString("Error in ActionHandler::discardReceiveFNcoordinates: was expecting coordinates for neither/both domain and target"));
    }
    if (expectingFNdomain)
    {
        isRhoDomainSet = false;
        inputMenu->setRhoDomainComboBox->setCurrentIndex(SET_RHO_CHOOSE);
        topFactory->subfactory.resetRhoDomain();
        expectingFNdomain = false;
    }
    if (expectingFNtarget)
    {
        isRhoImageSet = false;
        inputMenu->setRhoImageComboBox->setCurrentIndex(SET_RHO_CHOOSE);
        topFactory->subfactory.resetRhoTarget();
        expectingFNtarget = false;
    }
    window->setEnabled(true);
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
            setDisplayMenuReady(true);
            outputMenu->setEnabled(true);
            setReadyToCompute();
        }
        else
        {
            rightCanvas->setEnabled(false);
            rightDelegate->setIsRhoEmpty(true);
            rightDelegate->setIsFunctionEmpty(true);
            setDisplayMenuReady(true);
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

void ActionHandler::setDisplayMenuReady(bool left)
{
    displayMenu->setReady(left);
}

bool ActionHandler::isReadyToCompute() const
{
    bool test1 = (isRhoDomainSet && isRhoImageSet);
    bool test2 = topFactory->isFunctionInitialized();
    bool test3 = !(leftDelegate->buffer.isMeshEmpty || rightDelegate->buffer.isFunctionEmpty);
    //bool test4 = outputMenu->isEnabled();
    bool test5 = leftCanvas->isEnabled() && rightCanvas->isEnabled();

    bool test = test1 || test2 || test3 || test5;
    if (test != (test1 && test2 && test3 && test5))
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

void ActionHandler::randomFNcoordinates(int genus, std::vector<double> &lengthsOut, std::vector<double> &twistsOut)
{
    lengthsOut.clear();
    twistsOut.clear();
    int N = 3*genus - 3;
    lengthsOut.reserve(N);
    twistsOut.reserve(N);

    for (int i=0; i!=N; ++i)
    {
        lengthsOut.push_back(Tools::randDouble(1.0, 4.0));
        twistsOut.push_back(Tools::randDouble(-0.5, 0.5));
    }

    std::cout << "Chosen lengths and twists in selector: " << std::endl;
    std::cout << "lengths = " << lengthsOut << std::endl;
    std::cout << "twists = " << twistsOut << std::endl;
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

