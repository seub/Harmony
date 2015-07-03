#include "mainwindow.h"

#include <QStatusBar>
#include <QGridLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>

#include "canvas.h"
#include "topmenu.h"
#include "leftmenu.h"
#include "h2canvasdelegate.h"
#include "statusbar.h"



MainWindow::MainWindow(ActionHandler *handler)
{
    setWindowTitle(tr("Equivariant Harmonic Maps"));


    //QIcon icon(":/images/icon.png");
    //setWindowIcon(icon);

    createWindow(handler);
    setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::createWindow(ActionHandler *handler)
{    


    leftMenu = new LeftMenu(this);
    leftMenu->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    leftCanvas = new Canvas(DelegateType::H2DELEGATE_GRAPH, this, true, false, handler);
    leftCanvas->setEnabled(false);
    leftCanvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftCanvas->setMinimumSize(QSize(canvasMinimumSize(), canvasMinimumSize()));

    rightCanvas = new Canvas(DelegateType::H2DELEGATE_GRAPH, this, false, true, handler);
    rightCanvas->setEnabled(false);
    rightCanvas->setMinimumSize(QSize(canvasMinimumSize(), canvasMinimumSize()));
    rightCanvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    statusBar= new StatusBar(this);

    topMenu = new TopMenu(this);

    layout = new QGridLayout;

    layout->setMargin(9);
    layout->setSpacing(6);
    layout->setMenuBar(topMenu);
    layout->addWidget(leftMenu, 0, 0, 2, 1);
    layout->addWidget(leftCanvas, 0, 1, 2, 1);
    layout->addWidget(rightCanvas, 0, 2, 2, 1);
    layout->addWidget(statusBar, 2, 0, 1, 3);
    setLayout(layout);

    verticalCanvases = false;

    resize(optimalSize());
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

void MainWindow::enableCanvasesUpdates(bool enable)
{
    leftCanvas->setUpdatesEnabled(enable);
    rightCanvas->setUpdatesEnabled(enable);
    if (enable)
    {
        leftCanvas->updateRefresh(true, true);
        rightCanvas->updateRefresh(true, true);
    }
}

int MainWindow::canvasMinimumSize() const
{
    return leftMenu->sizeHint().width();
}

QSize MainWindow::optimalSize() const
{
    int screenWidth = QApplication::desktop()->width();
    int screenHeight = QApplication::desktop()->height();

    int complementaryWidth = 2*layout->margin() + 2*layout->horizontalSpacing() + leftMenu->sizeHint().width();
    int complementaryHeight = 2*layout->margin() + 2*layout->verticalSpacing() + topMenu->sizeHint().height() + statusBar->height();

    int canvasOptimalSizeH = Tools::intRound((0.85*screenWidth - complementaryWidth)/2);
    int canvasOptimalSizeV = std::max(Tools::intRound(0.85*screenHeight -complementaryHeight), leftMenu->minimumHeight());
    int canvasOptimalSize =  std::min(canvasOptimalSizeH, canvasOptimalSizeV);

    return QSize(complementaryWidth + 2*canvasOptimalSize, complementaryHeight + canvasOptimalSize);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    int complementaryWidthH = 2*layout->margin() + 2*layout->horizontalSpacing() + leftMenu->sizeHint().width();
    int complementaryHeightV = 2*layout->margin() + 3*layout->verticalSpacing() + topMenu->sizeHint().height() + statusBar->height();
    int maxCanvasSizeH = (width() - complementaryWidthH)/2;
    int maxCanvasSizeV = (height() - complementaryHeightV)/2;


    if (verticalCanvases && (maxCanvasSizeH > maxCanvasSizeV))
    {
        layout->removeWidget(leftCanvas);
        layout->removeWidget(rightCanvas);
        layout->addWidget(leftCanvas, 0, 1, 2, 1);
        layout->addWidget(rightCanvas, 0, 2, 2, 1);
        verticalCanvases = false;
        updateGeometry();
    }
    else if ((!verticalCanvases) && (maxCanvasSizeH < maxCanvasSizeV))
    {
        layout->removeWidget(leftCanvas);
        layout->removeWidget(rightCanvas);
        layout->addWidget(leftCanvas, 0, 1, 1, 2);
        layout->addWidget(rightCanvas, 1, 1, 1, 2);
        verticalCanvases = true;
        updateGeometry();
    }
}
