#include "actionhandler.h"

#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>
#include <QStatusBar>

#include "topfactory.h"
#include "h2canvasdelegateliftedgraph.h"
#include "mathscontainer.h"
#include "window.h"
#include "canvas.h"
#include "outputmenu.h"
#include "inputmenu.h"
#include "displaymenu.h"
#include "fenchelnielsenuser.h"
#include "discreteheatflowfactory.h"


ActionHandler::ActionHandler()
{
    resetBooleans();
}

void ActionHandler::resetBooleans()
{
    isShowingLive = false;
    isRhoDomainSet = false;
    isRhoImageSet = false;

    showTranslatesAroundVertex = false;
    showTranslatesAroundVertices = false;

    expectingFNdomain = false;
    expectingFNimage = false;
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

void ActionHandler::setContainer(MathsContainer *mathsContainer)
{
    this->mathsContainer = mathsContainer;
}

void ActionHandler::resetDelegatePointers()
{
    leftDelegate = static_cast<H2CanvasDelegateLiftedGraph*>(leftCanvas->delegate);
    rightDelegate = static_cast<H2CanvasDelegateLiftedGraph*>(rightCanvas->delegate);

    leftDelegate->setRhoPointer(&(mathsContainer->rhoDomain));
    leftDelegate->setGraphPointer(&(mathsContainer->domainFunction));

    rightDelegate->setRhoPointer(&(mathsContainer->H2RhoImage));
    rightDelegate->setGraphPointer(&(mathsContainer->H2ImageFunction));

    leftCanvas->updateRefresh(true, true);
    rightCanvas->updateRefresh(true, true);
}

void ActionHandler::setFactory(TopFactory *topFactory)
{
    this->topFactory = topFactory;
    topFactory->setGenus(inputMenu->getGenus());
    topFactory->setMeshDepth(inputMenu->getMeshDepth());
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
    statusBar->showMessage(QString("Mesh created with %1 vertices.").arg(QString::number(nbMeshPoints)), 10000);
}

void ActionHandler::showStatusBarMessage(const QString &message, int timeout)
{
    statusBar->showMessage(message, timeout);
}

void ActionHandler::processMessage(ActionHandlerMessage message)
{
    bool isVertexHighlighted, isTriangleHighlighted;
    uint graphIndexHighlighted, index1, index2, index3;
    bool update;

    switch(message)
    {
    case ActionHandlerMessage::HIGHLIGHTED_LEFT:
        leftCanvas->update();
        if (isReadyToCompute())
        {
            leftDelegate->getGraphTriangleIndicesHighlighted(isTriangleHighlighted, index1, index2, index3);
            leftDelegate->getGraphIndexHighlighted(isVertexHighlighted, graphIndexHighlighted);
            rightDelegate->decideHighlightingTriangle(isTriangleHighlighted, update, index1, index2, index3);
            rightDelegate->decideHighlightingGraphPoints(isVertexHighlighted, update, graphIndexHighlighted);
            rightCanvas->update();
        }
        break;

    case ActionHandlerMessage::HIGHLIGHTED_RIGHT:
        rightCanvas->update();
        if (isReadyToCompute())
        {
            rightDelegate->getGraphTriangleIndicesHighlighted(isTriangleHighlighted, index1, index2, index3);
            rightDelegate->getGraphIndexHighlighted(isVertexHighlighted, graphIndexHighlighted);
            leftDelegate->decideHighlightingTriangle(isTriangleHighlighted, update, index1, index2, index3);
            leftDelegate->decideHighlightingGraphPoints(isVertexHighlighted, update, graphIndexHighlighted);
            leftCanvas->update();
        }
        break;

    case ActionHandlerMessage::END_CANVAS_REPAINT:
        if (isShowingLive)
        {
            topFactory->h2factory.factory.refreshImageFunction();
            updateCanvasGraph(false, true, false);
            rightCanvas->update();
        }
        break;

    case ActionHandlerMessage::FINISHED_COMPUTING:
        finishedComputing();
        break;

    case ActionHandlerMessage::TRIANGLES_REFERENCE_AREAS:
        rightDelegate->setGraphTrianglesReferenceAreas(leftDelegate->getGraphTrianglesReferenceAreas());
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
    topFactory->runH2Flow();
    if (isShowingLive)
    {
        rightDelegate->setSendEndRepaint(true);
        rightDelegate->setShowTranslates(false, false);
        rightCanvas->update();
    }
}

void ActionHandler::outputResetButtonClicked()
{
    topFactory->resetH2Flow();
    outputMenu->resetMenu();
    leftCanvas->resetView();
    rightCanvas->resetView();
    updateCanvasGraph(false, true, true);
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
    bool aroundVertexOld = showTranslatesAroundVertex, aroundVerticesOld = showTranslatesAroundVertices;
    bool aroundVertexNew, aroundVerticesNew;

    switch(choice)
    {
    case DisplayMenu::SHOW_TRANSLATES_DOMAIN:
        aroundVertexNew = false;
        aroundVerticesNew = false;
        break;

    case DisplayMenu::SHOW_TRANSLATES_VERTEX:
        aroundVertexNew = true;
        aroundVerticesNew = false;
        break;

    case DisplayMenu::SHOW_TRANSLATES_VERTICES:
        aroundVertexNew = false;
        aroundVerticesNew = true;
        break;

    default:
        throw(QString("Error in ActionHandler::showTranslatesClicked: not supposed to land here"));
    }

    if ((aroundVertexNew != aroundVertexOld) || (aroundVerticesNew != aroundVerticesOld))
    {
        showTranslatesAroundVertex = aroundVertexNew;
        showTranslatesAroundVertices = aroundVerticesNew;
        leftDelegate->setShowTranslates(aroundVertexNew, aroundVerticesNew);
        if (isRhoDomainSet)
        {
            updateCanvasGraph(true, false, true);
        }

        uint nbMeshpoints = leftDelegate->graph->getNbPoints(), nbGraphPointsTranslates, nbTranslations;
        if (aroundVertexNew)
        {
            nbTranslations = leftDelegate->translationsAroundVertex.size();
            nbGraphPointsTranslates = nbTranslations*nbMeshpoints;
            statusBar->showMessage(QString("Showing %1 mesh points and %2 translates").arg(QString::number(nbMeshpoints)).arg(QString::number(nbGraphPointsTranslates)), 7000);
        }
        else
        {
            nbTranslations = leftDelegate->translationsAroundVertices.size();
            nbGraphPointsTranslates = nbTranslations*nbMeshpoints;
            if (aroundVerticesNew)
            {
                statusBar->showMessage(QString("Showing %1 mesh points and %2 translates").arg(QString::number(nbMeshpoints)).arg(QString::number(nbGraphPointsTranslates)), 7000);
            }
            else statusBar->showMessage(QString("Showing %1 mesh points and %2 fundamental domain translates").
                                        arg(QString::number(nbMeshpoints)).arg(QString::number(nbTranslations)), 7000);
        }

        rightDelegate->setShowTranslates(aroundVertexNew, aroundVerticesNew);
        if (isReadyToCompute())
        {
            updateCanvasGraph(false, true, true);
        }
    }
}

void ActionHandler::coloringClicked(int choice)
{
    switch (choice)
    {
    case DisplayMenu::COLORING_NONE:
        leftDelegate->setFilledTriangles(false);
        rightDelegate->setFilledTriangles(false);
        break;

    case DisplayMenu::COLORING_PLAIN:
        leftDelegate->setFilledTriangles(true);
        rightDelegate->setFilledTriangles(true);
        break;

    default:
        throw(QString("Error in ActionHandler::coloringClicked: not supposed to land here"));
        break;
    }
    updateCanvasGraph(true, true, showTranslatesAroundVertex || showTranslatesAroundVertices);
}

void ActionHandler::setRhoDomainClicked(int choice)
{
    switch(choice)
    {
    case InputMenu::SET_RHO_CHOOSE:
        isRhoDomainSet = false;
        topFactory->resetRhoDomain();
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
        topFactory->resetRhoImage();
        dealRhosReady();
        break;

    case InputMenu::SET_RHO_NICE:
        setRhoNiceImage();
        dealRhosReady();
        break;

    case InputMenu::SET_RHO_RANDOM:
        setRhoRandomImage();
        dealRhosReady();
        break;

    case InputMenu::SET_RHO_FN:
        setRhoFNImage();
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
        topFactory->setNiceRhoDomain();
    }
    else
    {
        errorMessageForSetRhoNice();
        isRhoDomainSet = false;
        inputMenu->setRhoDomainComboBox->setCurrentIndex(InputMenu::SET_RHO_CHOOSE);
        topFactory->resetRhoImage();
    }
}

void ActionHandler::setRhoNiceImage()
{
    if (inputMenu->getGenus() == 2)
    {
        isRhoImageSet = true;
        topFactory->setNiceRhoImage();
    }
    else
    {
        errorMessageForSetRhoNice();
        isRhoImageSet = false;
        inputMenu->setRhoImageComboBox->setCurrentIndex(InputMenu::SET_RHO_CHOOSE);
        topFactory->resetRhoImage();
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
    topFactory->setRhoDomain(lengths, twists);
}

void ActionHandler::setRhoRandomImage()
{
    std::vector<double> lengths, twists;
    uint genus = inputMenu->getGenus();
    ActionHandler::randomFNcoordinates(genus, lengths, twists);
    isRhoImageSet = true;
    topFactory->setRhoImage(lengths, twists);
}

void ActionHandler::setRhoFNDomain()
{
    expectingFNdomain = true;
    window->setEnabled(false);
    FenchelNielsenUser *FNuser = new FenchelNielsenUser(this, inputMenu->getGenus());
    FNuser->show();
}

void ActionHandler::setRhoFNImage()
{
    expectingFNimage = true;
    window->setEnabled(false);
    FenchelNielsenUser *FNuser = new FenchelNielsenUser(this, inputMenu->getGenus());
    FNuser->show();
}

void ActionHandler::receiveFNcoordinates(const std::vector<double> &lengths, const std::vector<double> &twists)
{
    if (expectingFNdomain == expectingFNimage)
    {
        throw(QString("Error in ActionHandler::receiveFNcoordinates: was expecting coordinates for neither/both domain and image"));
    }
    if (expectingFNdomain)
    {
        isRhoDomainSet = true;
        topFactory->setRhoDomain(lengths, twists);
        expectingFNdomain = false;
    }
    if (expectingFNimage)
    {
        isRhoImageSet = true;
        topFactory->setRhoImage(lengths, twists);
        expectingFNimage = false;
    }
    window->setEnabled(true);
    dealRhosReady();
}

void ActionHandler::discardReceiveFNcoordinates()
{
    if (expectingFNdomain == expectingFNimage)
    {
        throw(QString("Error in ActionHandler::discardReceiveFNcoordinates: was expecting coordinates for neither/both domain and image"));
    }
    if (expectingFNdomain)
    {
        isRhoDomainSet = false;
        inputMenu->setRhoDomainComboBox->setCurrentIndex(InputMenu::SET_RHO_CHOOSE);
        topFactory->resetRhoDomain();
        expectingFNdomain = false;
    }
    if (expectingFNimage)
    {
        isRhoImageSet = false;
        inputMenu->setRhoImageComboBox->setCurrentIndex(InputMenu::SET_RHO_CHOOSE);
        topFactory->resetRhoImage();
        expectingFNimage = false;
    }
    window->setEnabled(true);
    dealRhosReady();
}


void ActionHandler::dealRhosReady()
{
    if (isRhoDomainSet)
    {
        leftDelegate->setIsRhoEmpty(false);
        leftDelegate->refreshRho();
        leftDelegate->setIsGraphEmpty(false);
        leftCanvas->setEnabled(true);
        updateCanvasGraph(true, false, true);

        if (isRhoImageSet)
        {
            rightDelegate->setIsRhoEmpty(false);
            rightDelegate->refreshRho();
            rightDelegate->setIsGraphEmpty(false);
            rightCanvas->setEnabled(true);

            updateCanvasGraph(false, true, true);
            setDisplayMenuReady(true);
            outputMenu->setEnabled(true);
            outputMenu->resetMenu();
            setReadyToCompute();
        }
        else
        {
            rightCanvas->setEnabled(false);
            rightDelegate->setIsRhoEmpty(true);
            rightDelegate->setIsGraphEmpty(true);
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
        leftDelegate->setIsGraphEmpty(true);

        rightDelegate->setIsRhoEmpty(!isRhoImageSet);
        if (isRhoImageSet)
        {
            rightDelegate->refreshRho();
        }
        rightDelegate->setIsGraphEmpty(true);
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
    bool test3 = !(leftDelegate->isGraphEmpty || rightDelegate->isGraphEmpty);

    bool test = test1 || test3;
    if (test != (test1 && test3))
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
    topFactory->stopH2Flow();
}

void ActionHandler::updateCanvasGraph(bool left, bool right, bool refreshFullTranslates)
{
    if (left)
    {
        leftDelegate->updateGraph(true, refreshFullTranslates);
        leftCanvas->updateRefresh(true, true);
    }
    if (right)
    {
        rightDelegate->updateGraph(true, refreshFullTranslates);
        rightCanvas->updateRefresh(true, true);
    }
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

void ActionHandler::finishedComputing()
{
    isShowingLive = false;
    leftCanvas->setEnabled(true);
    rightCanvas->setEnabled(true);
    inputMenu->setEnabled(true);
    displayMenu->setEnabled(true);
    outputMenu->switchStopToComputeButton();
    outputMenu->enableAll();
    rightDelegate->setShowTranslates(showTranslatesAroundVertex, showTranslatesAroundVertices);
    rightDelegate->setSendEndRepaint(false);
    updateCanvasGraph(false, true, true);
}

