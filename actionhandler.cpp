#include "actionhandler.h"

#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>
#include <QStatusBar>

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
    resetBooleans();
}

void ActionHandler::resetBooleans()
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
    statusBar = window->statusBar;
}

void ActionHandler::setContainer(MathsContainer *container)
{
    this->container = container;
    setFactory();
    resetDelegatePointers();
}

void ActionHandler::resetDelegatePointers()
{
    leftDelegate = static_cast<H2CanvasDelegate*> (leftCanvas->delegate);
    rightDelegate = static_cast<H2CanvasDelegate*> (rightCanvas->delegate);

    leftDelegate->buffer.setRhoPointer(&topFactory->subfactory.rhoDomain, "blue");
    leftDelegate->buffer.setMeshPointer(&topFactory->subfactory.mesh, "red");

    rightDelegate->buffer.setRhoPointer(&topFactory->subfactory.rhoTarget, "blue");
    rightDelegate->buffer.setFunctionPointer(&topFactory->subfactory.function, "red");

    leftCanvas->updateRefresh(true, true);
    rightCanvas->updateRefresh(true, true);
}

void ActionHandler::setFactory()
{
    topFactory = &(this->container->topFactory);
    topFactory->setHandler(this);
    topFactory->setGenus(inputMenu->getGenus());
    topFactory->setMeshDepth(inputMenu->getMeshDepth());
    connect(&(topFactory->subfactory), SIGNAL(meshCreated(uint)), this, SLOT(meshCreated(uint)));
    resetDelegatePointers();
}

void ActionHandler::setReadyToCompute()
{
    if (!isReadyToCompute())
    {
        throw(QString("Error in ActionHandler::setReadyToCompute: test failed"));
    }
}

void ActionHandler::meshCreated(uint nbMeshPoints)
{
    statusBar->showMessage(QString("Mesh created with %1 meshpoints").arg(QString::number(nbMeshPoints)), 7000);
}

void ActionHandler::processMessage(ActionHandlerMessage message, int)
{
    bool isVertexHighlighted, isTriangleHighlighted;
    uint meshIndexHighlighted, index1, index2, index3;
    bool update;

    switch(message)
    {
    case ActionHandlerMessage::HIGHLIGHTED_LEFT:
        leftCanvas->update();
        if (isReadyToCompute())
        {
            leftDelegate->getMeshTriangleIndicesHighlighted(isTriangleHighlighted, index1, index2, index3);
            leftDelegate->getMeshIndexHighlighted(isVertexHighlighted, meshIndexHighlighted);
            rightDelegate->decideHighlightingTriangle(isTriangleHighlighted, update, index1, index2, index3);
            rightDelegate->decideHighlightingMeshPoints(isVertexHighlighted, update, meshIndexHighlighted);
            rightCanvas->update();
        }
        break;

    case ActionHandlerMessage::HIGHLIGHTED_RIGHT:
        rightCanvas->update();
        if (isReadyToCompute())
        {
            rightDelegate->getMeshTriangleIndicesHighlighted(isTriangleHighlighted, index1, index2, index3);
            rightDelegate->getMeshIndexHighlighted(isVertexHighlighted, meshIndexHighlighted);
            leftDelegate->decideHighlightingTriangle(isTriangleHighlighted, update, index1, index2, index3);
            leftDelegate->decideHighlightingMeshPoints(isVertexHighlighted, update, meshIndexHighlighted);
            leftCanvas->update();
        }
        break;

    case ActionHandlerMessage::END_TARGET_CANVAS_REPAINT:
        if (isShowingLive)
        {
            topFactory->subfactory.refreshFunction();
            updateFunction(false);
            rightCanvas->update();
        }
        break;

    case ActionHandlerMessage::FINISHED_COMPUTING:
        finishedComputing();
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
        rightCanvas->update();
    }
}

void ActionHandler::outputResetButtonClicked()
{
    topFactory->resetInitSubfactory();
    outputMenu->resetMenu();
    leftCanvas->resetView();
    rightCanvas->resetView();
    updateFunction(true);
}

void ActionHandler::iterateButtonClicked()
{
    uint N = outputMenu->getNbIterations();
    topFactory->iterateSubfactory(N);
    statusBar->showMessage(QString("%1 iterations computed in %2s")
                           .arg(QString::number(N))
                           .arg(QString::number(topFactory->getTimeElapsed())), 7000);
    updateFunction(true);
    outputMenu->enableReset();
}

void ActionHandler::resetViewButtonClicked()
{
    leftCanvas->resetView();
    rightCanvas->resetView();
    leftCanvas->update();
    rightCanvas->update();
}

void ActionHandler::showTranslatesClicked(int choice)
{
    bool aroundVertexOldLeft = showTranslatesAroundVertexLeft, aroundVerticesOldLeft = showTranslatesAroundVerticesLeft;
    bool aroundVertexOldRight = showTranslatesAroundVertexRight, aroundVerticesOldRight = showTranslatesAroundVerticesRight;
    bool aroundVertexNewLeft, aroundVerticesNewLeft, aroundVertexNewRight, aroundVerticesNewRight;

    switch(choice)
    {
    case DisplayMenu::SHOW_TRANSLATES_DOMAIN:
        aroundVertexNewLeft = false;
        aroundVerticesNewLeft = false;
        aroundVertexNewRight = false;
        aroundVerticesNewRight = false;
        break;

    case DisplayMenu::SHOW_TRANSLATES_VERTEX:
        aroundVertexNewLeft = true;
        aroundVerticesNewLeft = false;
        aroundVertexNewRight = true;
        aroundVerticesNewRight = false;
        break;

    case DisplayMenu::SHOW_TRANSLATES_VERTICES:
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

        uint nbMeshpoints = leftDelegate->buffer.mesh->nbPoints(), nbMeshPointsTranslates, nbTranslations;
        if (aroundVertexNewLeft)
        {
            nbTranslations = leftDelegate->buffer.translationsAroundVertex.size();
            nbMeshPointsTranslates = nbTranslations*nbMeshpoints;
            statusBar->showMessage(QString("Showing %1 mesh points and %2 translates").arg(QString::number(nbMeshpoints)).arg(QString::number(nbMeshPointsTranslates)), 7000);
        }
        else
        {
            nbTranslations = leftDelegate->buffer.translationsAroundVertices.size();
            nbMeshPointsTranslates = nbTranslations*nbMeshpoints;
            if (aroundVerticesNewLeft)
            {
                statusBar->showMessage(QString("Showing %1 mesh points and %2 translates").arg(QString::number(nbMeshpoints)).arg(QString::number(nbMeshPointsTranslates)), 7000);
            }
            else statusBar->showMessage(QString("Showing %1 mesh points and %2 fundamental domain translates").
                                        arg(QString::number(nbMeshpoints)).arg(QString::number(nbTranslations)), 7000);
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
    switch(choice)
    {
    case InputMenu::SET_RHO_CHOOSE:
        isRhoDomainSet = false;
        topFactory->subfactory.resetRhoDomain();
        break;

    case InputMenu::SET_RHO_NICE:
        setRhoNiceDomain();
        break;

    case InputMenu::SET_RHO_RANDOM:
        setRhoRandomDomain();
        break;

    case InputMenu::SET_RHO_FN:
        setRhoFNDomain();
        break;

    default:
        throw(QString("Error in ActionHandler::setRhoDomainClicked: not supposed to land here"));
    }

    dealRhosReady();
}

void ActionHandler::setRhoImageClicked(int choice)
{

    switch(choice)
    {
    case InputMenu::SET_RHO_CHOOSE:
        isRhoImageSet = false;
        topFactory->subfactory.resetRhoTarget();
        dealRhosReady();
        break;

    case InputMenu::SET_RHO_NICE:
        setRhoNiceTarget();
        dealRhosReady();
        break;

    case InputMenu::SET_RHO_RANDOM:
        setRhoRandomTarget();
        dealRhosReady();
        break;

    case InputMenu::SET_RHO_FN:
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
        inputMenu->setRhoDomainComboBox->setCurrentIndex(InputMenu::SET_RHO_CHOOSE);
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
        inputMenu->setRhoImageComboBox->setCurrentIndex(InputMenu::SET_RHO_CHOOSE);
        topFactory->subfactory.resetRhoTarget();
    }
}

void ActionHandler::errorMessageForSetRhoNice()
{
    QMessageBox messageBox;
    QString message = QString("%1Nice%2 representation is only available for genus 2").arg(QChar(8220)).arg(QChar(8221));


    QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setInformativeText(message);

    QGridLayout* layout = dynamic_cast<QGridLayout*>(messageBox.layout());
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    messageBox.setAttribute(Qt::WA_DeleteOnClose);
    messageBox.exec();

}


void ActionHandler::setRhoRandomDomain()
{
    std::vector<double> lengths, twists;
    uint genus = inputMenu->getGenus();
    ActionHandler::randomFNcoordinates(genus, lengths, twists);
    isRhoDomainSet = true;
    topFactory->subfactory.setRhoDomain(lengths, twists);
}

void ActionHandler::setRhoRandomTarget()
{
    std::vector<double> lengths, twists;
    uint genus = inputMenu->getGenus();
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
        inputMenu->setRhoDomainComboBox->setCurrentIndex(InputMenu::SET_RHO_CHOOSE);
        topFactory->subfactory.resetRhoDomain();
        expectingFNdomain = false;
    }
    if (expectingFNtarget)
    {
        isRhoImageSet = false;
        inputMenu->setRhoImageComboBox->setCurrentIndex(InputMenu::SET_RHO_CHOOSE);
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

    leftCanvas->updateRefresh(true, true);
    rightCanvas->updateRefresh(true, true);
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

    (static_cast<H2CanvasDelegateDomain *> (leftDelegate))->refreshMesh();
    if (updateTranslates)
    {
        leftDelegate->refreshTranslates();
    }
    else
    {
        leftDelegate->refreshTranslates(false, false);
    }
    leftCanvas->updateRefresh(true, true);
    rightCanvas->updateRefresh(true, true);
}

void ActionHandler::randomFNcoordinates(uint genus, std::vector<double> &lengthsOut, std::vector<double> &twistsOut)
{
    lengthsOut.clear();
    twistsOut.clear();
    uint N = 3*genus - 3;
    lengthsOut.reserve(N);
    twistsOut.reserve(N);

    for (uint i=0; i!=N; ++i)
    {
        lengthsOut.push_back(Tools::randDouble(1.0, 4.0));
        twistsOut.push_back(Tools::randDouble(-0.5, 0.5));
    }
}

void ActionHandler::updateFunction(bool updateTranslates)
{
    if (!topFactory->isFunctionInitialized())
    {
        throw(QString("Error in ActionHandler::updateFunction: not supposed to happen"));
    }

    static_cast<H2CanvasDelegateTarget *>(rightDelegate)->refreshFunction();
    if (updateTranslates)
    {
        rightDelegate->refreshTranslates();
    }
    else
    {
        rightDelegate->refreshTranslates(false, false);
    }
    leftCanvas->updateRefresh(true, true);
    rightCanvas->updateRefresh(true, true);
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

    statusBar->showMessage(QString("%1 iterations computed in %2s")
                           .arg(QString::number(topFactory->getNbIterations()))
                           .arg(QString::number(topFactory->getTimeElapsed())), 7000);

    rightDelegate->setShowTranslates(showTranslatesAroundVertexRight, showTranslatesAroundVerticesRight);
    updateFunction(true);
}
