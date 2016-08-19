#include "actionhandler.h"

#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>
#include <QStatusBar>
#include <QColor>
#include <QPushButton>
#include <QSpinBox>

#include "topfactory.h"
#include "h2canvasdelegateliftedgraph.h"
#include "mathscontainer.h"
#include "mainwindow.h"
#include "canvas.h"
#include "outputmenu.h"
#include "inputmenu.h"
#include "displaymenu.h"
#include "leftmenu.h"
#include "fenchelnielsenuser.h"
#include "discreteflowfactory.h"
#include "statusbar.h"


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
    showTranslatesAroundVerticesStar = false;
    
    expectingFNdomain = false;
    expectingFNimage = false;
}

void ActionHandler::setWindow(MainWindow *window)
{
    this->window = window;
    inputMenu = window->leftMenu->inputMenu;
    displayMenu = window->leftMenu->displayMenu;
    outputMenu = window->leftMenu->outputMenu;
    leftCanvas = window->leftCanvas;
    rightCanvas = window->rightCanvas;
    statusBar = window->statusBar;

    connectMenusSignals();
}

void ActionHandler::connectMenusSignals()
{
    connect(inputMenu->genusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(genusClicked(int)));
    connect(inputMenu->setRhoDomainComboBox, SIGNAL(activated(int)), this, SLOT(setRhoDomainClicked(int)));
    connect(inputMenu->setRhoImageComboBox, SIGNAL(activated(int)), this, SLOT(setRhoImageClicked(int)));
    connect(inputMenu->meshDepthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(meshDepthClicked(int)));

    connect(displayMenu->resetViewButton, SIGNAL(clicked()), this, SLOT(resetViewButtonClicked()));
    connect(displayMenu->showTranslatesComboBox, SIGNAL(activated(int)), this, SLOT(showTranslatesClicked(int)));
    //connect(displayMenu->coloringComboBox, SIGNAL(activated(int)), this, SLOT(coloringClicked(int)));
    //connect(displayMenu->colorComboBox, SIGNAL(activated(int)), this, SLOT(colorClicked(int)));

    connect(displayMenu->coloringComboBoxLeft, SIGNAL(activated(int)), this, SLOT(coloringClickedLeft(int)));
    connect(displayMenu->colorComboBoxLeft, SIGNAL(activated(int)), this, SLOT(colorClickedLeft(int)));
    connect(displayMenu->coloringComboBoxRight, SIGNAL(activated(int)), this, SLOT(coloringClickedRight(int)));
    connect(displayMenu->colorComboBoxRight, SIGNAL(activated(int)), this, SLOT(colorClickedRight(int)));

    connect(outputMenu->computeButton, SIGNAL(clicked()), this, SLOT(computeButtonClicked()));
    connect(outputMenu->iterateButton, SIGNAL(clicked()), this, SLOT(iterateButtonClicked()));
    connect(outputMenu->resetButton, SIGNAL(clicked()), this, SLOT(outputResetButtonClicked()));

    connect(outputMenu->flowComboBox, SIGNAL(activated(int)), this, SLOT(flowChoiceClicked(int)));
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

    leftDelegate->domainTrianglesAreas = std::make_shared< std::vector<double> >();
    rightDelegate->domainTrianglesAreas = leftDelegate->domainTrianglesAreas;
    
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
    bool isTriangleHighlighted;
    uint index1, index2, index3;
    bool update;
    
    switch(message)
    {
    case ActionHandlerMessage::HIGHLIGHTED_LEFT:
        leftCanvas->update();
        if (isReadyToCompute())
        {
            leftDelegate->getGraphTriangleIndicesHighlighted(isTriangleHighlighted, index1, index2, index3);
            rightDelegate->decideHighlightingTriangle(isTriangleHighlighted, update, index1, index2, index3);
            rightCanvas->update();
        }
        break;
        
    case ActionHandlerMessage::HIGHLIGHTED_RIGHT:
        rightDelegate->getGraphTriangleIndicesHighlighted(isTriangleHighlighted, index1, index2, index3);
        leftDelegate->decideHighlightingTriangle(isTriangleHighlighted, update, index1, index2, index3);
        leftCanvas->update();
        break;
        
    case ActionHandlerMessage::END_CANVAS_REPAINT:
        if (isShowingLive)
        {
            topFactory->h2factory.factory.refreshImageFunction();
            updateCanvasGraph(false, true);
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

    disconnect(outputMenu->computeButton, SIGNAL(clicked()), this, SLOT(computeButtonClicked()));
    connect(outputMenu->computeButton, SIGNAL(clicked()), this, SLOT(stopButtonClicked()));

    isShowingLive = outputMenu->showLiveCheckbox->checkState();
    topFactory->runH2Flow(outputMenu->flowComboBox->currentIndex());
    if (isShowingLive)
    {
        rightDelegate->setSendEndRepaint(true);
        rightDelegate->setShowTranslates(false, false, false);
        rightCanvas->update();
    }
}

void ActionHandler::iterateButtonClicked()
{
    leftCanvas->setEnabled(false);
    rightCanvas->setEnabled(false);
    inputMenu->setEnabled(false);
    displayMenu->setEnabled(false);
    outputMenu->disableAllButStop();
    outputMenu->update();

    topFactory->iterateH2Flow(outputMenu->flowComboBox->currentIndex(),outputMenu->iterateSpinBox->value());
}


void ActionHandler::outputResetButtonClicked()
{
    topFactory->resetH2Flow();
    outputMenu->resetMenu();
    leftCanvas->resetView();
    rightCanvas->resetView();
    updateCanvasGraph(false, true);
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
    bool aroundVertexOld = showTranslatesAroundVertex, aroundVerticesOld = showTranslatesAroundVertices, aroundVerticesStarOld = showTranslatesAroundVerticesStar;
    bool aroundVertexNew, aroundVerticesNew, aroundVerticesStarNew;
    
    switch(choice)
    {
    case DisplayMenu::SHOW_TRANSLATES_DOMAIN:
        aroundVertexNew = false;
        aroundVerticesNew = false;
        aroundVerticesStarNew = false;
        break;
        
    case DisplayMenu::SHOW_TRANSLATES_VERTEX:
        aroundVertexNew = true;
        aroundVerticesNew = false;
        aroundVerticesStarNew = false;
        break;
        
    case DisplayMenu::SHOW_TRANSLATES_VERTICES:
        aroundVertexNew = false;
        aroundVerticesNew = true;
        aroundVerticesStarNew = false;
        break;

    case DisplayMenu::SHOW_TRANSLATES_VERTICES_STAR:
        aroundVertexNew = false;
        aroundVerticesNew = true;
        aroundVerticesStarNew = true;
        break;
        
    default:
        throw(QString("Error in ActionHandler::showTranslatesClicked: not supposed to land here"));
    }
    
    if ((aroundVertexNew != aroundVertexOld) || (aroundVerticesNew != aroundVerticesOld) || (aroundVerticesStarNew != aroundVerticesStarOld))
    {
        showTranslatesAroundVertex = aroundVertexNew;
        showTranslatesAroundVertices = aroundVerticesNew;
        showTranslatesAroundVerticesStar = aroundVerticesStarNew;
        leftDelegate->setShowTranslates(aroundVertexNew, aroundVerticesNew, aroundVerticesStarNew);
        if (isRhoDomainSet)
        {
            updateCanvasGraph(true, false);
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
        
        rightDelegate->setShowTranslates(aroundVertexNew, aroundVerticesNew, aroundVerticesStarNew);
        if (isReadyToCompute())
        {
            updateCanvasGraph(false, true);
        }
    }
}
/*
void ActionHandler::coloringClicked(int choice)
{
    switch (choice)
    {
    case DisplayMenu::COLORING_NONE:
        leftDelegate->setFilledTriangles(false);
        rightDelegate->setFilledTriangles(false);
        break;
        
    case DisplayMenu::COLORING_PLAIN:
        //leftDelegate->setFilledTriangles(true);
        leftDelegate->setFilledTriangles(true);
        rightDelegate->setFilledTriangles(true);
        break;
        
    default:
        throw(QString("Error in ActionHandler::coloringClicked: not supposed to land here"));
        break;
    }
    updateCanvasGraph(true, true);
}

void ActionHandler::colorClicked(int choice)
{
    switch (choice)
    {
    case DisplayMenu::RED:
        leftDelegate->setGraphColor("red");
        rightDelegate->setGraphColor("red");
        break;

    case DisplayMenu::GREEN:
        leftDelegate->setGraphColor("green");
        rightDelegate->setGraphColor("green");
        break;

    case DisplayMenu::BLUE:
        leftDelegate->setGraphColor("blue");
        rightDelegate->setGraphColor("blue");
        break;

    case DisplayMenu::LIGHT_BLUE:
        leftDelegate->setGraphColor(QColor(9, 118, 227));
        rightDelegate->setGraphColor(QColor(9, 118, 227));
        break;

    case DisplayMenu::ORANGE:
        leftDelegate->setGraphColor("orange");
        rightDelegate->setGraphColor("orange");
        break;

    case DisplayMenu::GRAY:
        leftDelegate->setGraphColor(QColor(170, 170, 170));
        rightDelegate->setGraphColor(QColor(170, 170, 170));
        break;

    case DisplayMenu::BLACK:
        leftDelegate->setGraphColor("black");
        rightDelegate->setGraphColor("black");
        break;

    default:
        throw(QString("Error in ActionHandler::colorClicked: not supposed to land here"));
        break;
    }
    updateCanvasGraph(true, true);
}
*/


void ActionHandler::coloringClickedLeft(int choice)
{
    switch (choice)
    {
    case DisplayMenu::COLORING_NONE_LEFT:
        leftDelegate->setFilledTriangles(false);
        break;

    case DisplayMenu::COLORING_PLAIN_LEFT:
        leftDelegate->setFilledTriangles(true);
        break;

    default:
        throw(QString("Error in ActionHandler::coloringClickedLeft: not supposed to land here"));
        break;
    }
    updateCanvasGraph(true, false);
}

void ActionHandler::coloringClickedRight(int choice)
{
    switch (choice)
    {
    case DisplayMenu::COLORING_NONE_RIGHT:
        rightDelegate->setFilledTriangles(false);
        break;

    case DisplayMenu::COLORING_PLAIN_RIGHT:
        rightDelegate->setFilledTriangles(true);
        break;

    default:
        throw(QString("Error in ActionHandler::coloringClickedRight: not supposed to land here"));
        break;
    }
    updateCanvasGraph(false, true);
}


void ActionHandler::colorClickedLeft(int choice)
{
    switch (choice)
    {
    case DisplayMenu::RED_L:
        leftDelegate->setGraphColor("red");
        break;

    case DisplayMenu::GREEN_L:
        leftDelegate->setGraphColor("green");
        break;

    case DisplayMenu::BLUE_L:
        leftDelegate->setGraphColor("blue");
        break;

    case DisplayMenu::LIGHT_BLUE_L:
        leftDelegate->setGraphColor(QColor(9, 118, 227));
        break;

    case DisplayMenu::ORANGE_L:
        leftDelegate->setGraphColor("orange");
        break;

    case DisplayMenu::GRAY_L:
        leftDelegate->setGraphColor(QColor(170, 170, 170));
        break;

    case DisplayMenu::BLACK_L:
        leftDelegate->setGraphColor("black");
        break;

    default:
        throw(QString("Error in ActionHandler::colorClickedLeft: not supposed to land here"));
        break;
    }
    updateCanvasGraph(true, false);
}

void ActionHandler::colorClickedRight(int choice)
{
    switch (choice)
    {
    case DisplayMenu::RED_R:
        rightDelegate->setGraphColor("red");
        break;

    case DisplayMenu::GREEN_R:
        rightDelegate->setGraphColor("green");
        break;

    case DisplayMenu::BLUE_R:
        rightDelegate->setGraphColor("blue");
        break;

    case DisplayMenu::LIGHT_BLUE_R:
        rightDelegate->setGraphColor(QColor(9, 118, 227));
        break;

    case DisplayMenu::ORANGE_R:
        rightDelegate->setGraphColor("orange");
        break;

    case DisplayMenu::GRAY_R:
        rightDelegate->setGraphColor(QColor(170, 170, 170));
        break;

    case DisplayMenu::BLACK_R:
        rightDelegate->setGraphColor("black");
        break;

    default:
        throw(QString("Error in ActionHandler::colorClickedRight: not supposed to land here"));
        break;
    }
    updateCanvasGraph(false, true);
}


void ActionHandler::flowChoiceClicked(int choice)
{
    switch(choice)
    {
    case OutputMenu::FLOW_CHOICE:
        outputMenu->enableRunButtons(false);
        break;

    case OutputMenu::FLOW_CENTROID:
        outputMenu->enableRunButtons(true);
        break;

    case OutputMenu::FLOW_ENERGY_CONSTANT_STEP:
        outputMenu->enableRunButtons(true);
        break;

    default:
        throw(QString("Error in ActionHandler: flowChoice issues."));
    }
}

void ActionHandler::setRhoDomainClicked(int choice)
{
    switch(choice)
    {
    case InputMenu::SET_RHO_CHOOSE:
        isRhoDomainSet = false;
        topFactory->resetRhoDomain();
        dealRhoDomainReady();
        break;
        
    case InputMenu::SET_RHO_NICE:
        setRhoNiceDomain();
        dealRhoDomainReady();
        break;
        
    case InputMenu::SET_RHO_RANDOM:
        setRhoRandomDomain();
        dealRhoDomainReady();
        break;
        
    case InputMenu::SET_RHO_FN:
        setRhoFNDomain();
        break;
        
    default:
        throw(QString("Error in ActionHandler::setRhoDomainClicked: not supposed to land here"));
    }
}

void ActionHandler::setRhoImageClicked(int choice)
{
    
    switch(choice)
    {
    case InputMenu::SET_RHO_CHOOSE:
        isRhoImageSet = false;
        topFactory->resetRhoImage();
        dealRhoImageReady();
        break;
        
    case InputMenu::SET_RHO_NICE:
        setRhoNiceImage();
        dealRhoImageReady();
        break;
        
    case InputMenu::SET_RHO_RANDOM:
        setRhoRandomImage();
        dealRhoImageReady();
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
        dealRhoDomainReady();
    }
    if (expectingFNimage)
    {
        isRhoImageSet = true;
        topFactory->setRhoImage(lengths, twists);
        expectingFNimage = false;
        dealRhoImageReady();
    }
    window->setEnabled(true);
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
        dealRhoDomainReady();
    }
    if (expectingFNimage)
    {
        isRhoImageSet = false;
        inputMenu->setRhoImageComboBox->setCurrentIndex(InputMenu::SET_RHO_CHOOSE);
        topFactory->resetRhoImage();
        expectingFNimage = false;
        dealRhoImageReady();
    }
    window->setEnabled(true);
}


void ActionHandler::dealRhoDomainReady()
{
    if (isRhoDomainSet)
    {
        leftDelegate->setIsRhoEmpty(false);
        leftDelegate->refreshRho();
        leftDelegate->setIsGraphEmpty(false);
        leftCanvas->setEnabled(true);
    }    
    updateCanvasGraph(true, false);
    dealRhoImageReady();
}

void ActionHandler::dealRhoImageReady()
{
    if (isRhoDomainSet)
    {
        if (isRhoImageSet)
        {
            rightDelegate->setIsRhoEmpty(false);
            rightDelegate->refreshRho();
            rightDelegate->setIsGraphEmpty(false);
            rightCanvas->setEnabled(true);
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
    updateCanvasGraph(false, true);
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
    
    dealRhoDomainReady();
}

void ActionHandler::meshDepthClicked(int choice)
{
    topFactory->setMeshDepth(choice);
    dealRhoDomainReady();
}

void ActionHandler::stopButtonClicked()
{
    topFactory->stopH2Flow();
}

void ActionHandler::updateCanvasGraph(bool left, bool right)
{
    if (left)
    {
        leftDelegate->updateGraph(true);
        leftCanvas->updateRefresh(true, true);
    }
    if (right)
    {
        rightDelegate->updateGraph(true);
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
    disconnect(outputMenu->computeButton, SIGNAL(clicked()), this, SLOT(stopButtonClicked()));
    connect(outputMenu->computeButton, SIGNAL(clicked()), this, SLOT(computeButtonClicked()));

    outputMenu->enableAll();
    rightDelegate->setShowTranslates(showTranslatesAroundVertex, showTranslatesAroundVertices, showTranslatesAroundVerticesStar);
    rightDelegate->setSendEndRepaint(false);
    updateCanvasGraph(false, true);
}

