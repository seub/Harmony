#include "window.h"

#include <QLabel>
#include <QStatusBar>
#include <QGridLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>

#include "canvas.h"
#include "topmenu.h"
#include "inputmenu.h"
#include "outputmenu.h"
#include "displaymenu.h"
#include "h2canvasdelegate.h"
#include "statusbar.h"



Window::Window(ActionHandler *handler)
{
    createWindow(handler);
    setAttribute(Qt::WA_DeleteOnClose);
}

void Window::createWindow(ActionHandler *handler)
{
    leftCanvas = new Canvas(DelegateType::H2DELEGATE_GRAPH, this, true, false, handler);
    leftCanvas->setEnabled(false);

    rightCanvas = new Canvas(DelegateType::H2DELEGATE_GRAPH, this, false, true, handler);
    rightCanvas->setEnabled(false);

    inputMenu = new InputMenu(this, handler);
    inputMenu->setEnabled(true);

    displayMenu = new DisplayMenu(this, handler);
    displayMenu->setEnabled(false);

    outputMenu = new OutputMenu(this, handler);
    outputMenu->setEnabled(false);

    statusBar= new StatusBar(this);
    statusBar->setSizeGripEnabled(false);
    statusBar->setFixedHeight(25);
    statusBarLabel = new QLabel;
    statusBarLabel->setText("Ready");
    statusBar->addWidget(statusBarLabel);
    topMenu = new TopMenu(this);
    layout = new QGridLayout;

    layout->setMargin(9);
    layout->setSpacing(6);
    layout->setMenuBar(topMenu);


    layout->setColumnMinimumWidth(0, menuWidth());
    int leftColMenuSize = std::max(std::max(inputMenu->maxLeftColWidth(), outputMenu->maxLeftColWidth()), displayMenu->maxLeftColWidth());
    inputMenu->layout->setColumnMinimumWidth(0, leftColMenuSize);
    displayMenu->layout->setColumnMinimumWidth(0, leftColMenuSize);
    outputMenu->layout->setColumnMinimumWidth(0, leftColMenuSize);


    layout->addWidget(leftCanvas, 0, 1, 3, 1);
    layout->addWidget(rightCanvas, 0, 2, 3, 1);
    layout->addWidget(inputMenu, 0, 0, Qt::AlignTop);
    layout->addWidget(outputMenu, 1, 0, Qt::AlignTop);
    layout->addWidget(displayMenu, 2, 0, Qt::AlignTop);
    layout->addWidget(statusBar, 3, 0, 1, 3);
    setLayout(layout);


    setWindowTitle(tr("Equivariant Harmonic Maps"));

    //QIcon icon(":/images/icon.png");
    //setWindowIcon(icon);

    uint w, h;
    optimalSize(w,h);
    resize(w, h);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

void Window::resizeEvent(QResizeEvent * event)
{

    int deltaWidth = abs(event->size().width() - event->oldSize().width());
    int deltaHeight = abs(event->size().height() - event->oldSize().height());

    if (deltaWidth >= 1 || deltaHeight >= 1 || leftCanvas->width()!=leftCanvas->height())
    {
        enableCanvasesUpdates(false);
        canvasResized();
        enableCanvasesUpdates(true);
    }
    leftCanvas->updateRefresh(true, true);
    rightCanvas->updateRefresh(true, true);
}

void Window::enableCanvasesUpdates(bool b)
{
    leftCanvas->setUpdatesEnabled(b);
    rightCanvas->setUpdatesEnabled(b);
    if (b)
    {
        leftCanvas->updateRefresh(true, true);
        rightCanvas->updateRefresh(true, true);
    }
}

void Window::canvasResized()
{
    int margin = layout->margin();
    int extMarginV = std::max(inputMenu->geometry().top(), margin);
    int extMarginH = inputMenu->geometry().left();
    int statusBarHeight = statusBar->height();
    //int inputMenuWidth = inputMenu->width();
    int inputMenuWidth = menuWidth();
    int interWidth = layout->horizontalSpacing();
    int interHeight = layout->verticalSpacing();

    int maxWidthH = (width() - (2*extMarginH + inputMenuWidth + 2*interWidth))/2;
    int maxHeightH = height() - (statusBarHeight + interHeight + extMarginV + margin);

    int newSizeH, extraWidthH, extraHeightH;
    if (maxWidthH < maxHeightH)
    {
        newSizeH = maxWidthH;
        extraWidthH = 0;
        extraHeightH = std::max(height() - (statusBarHeight + interHeight + extMarginV + margin) - newSizeH, 0);
    }
    else
    {
        newSizeH = maxHeightH;
        extraHeightH = 0;
        extraWidthH = std::max(width() - (2*extMarginH + inputMenuWidth + 2*interWidth) - 2*newSizeH, 0);
    }

    int maxWidthV = width() - (2*extMarginH + inputMenuWidth + interWidth);
    int maxHeightV = (height() - (statusBarHeight + 2*interHeight + extMarginV + margin))/2;
    int newSizeV, extraWidthV, extraHeightV;
    if (maxWidthV < maxHeightV)
    {
        newSizeV = maxWidthV;
        extraWidthV = 0;
        extraHeightV = std::max(height() - (statusBarHeight + 2*interHeight + extMarginV + margin) - 2*newSizeV, 0);
    }
    else
    {
        newSizeV = maxHeightV;
        extraHeightV = 0;
        extraWidthV = std::max(width() - (2*extMarginH + inputMenuWidth + interWidth) - newSizeV, 0);
    }


    int wh = extraWidthH/3;
    int hh = extraHeightH/2;
    int wv = extraWidthV/2;
    int hv = extraHeightV/3;

    int leftL, topL, leftR, topR, size;
    if (newSizeV > newSizeH)
    {
        leftL = (extMarginH + inputMenuWidth + interWidth) + wv;
        topL = extMarginV + hv;
        leftR = (extMarginH + inputMenuWidth + interWidth) + wv;
        topR = extMarginV + hv + newSizeV + interHeight + hv;
        size = newSizeV;

    }
    else
    {
        leftL = (extMarginH + inputMenuWidth + interWidth) + wh;
        topL = extMarginV + hh;
        leftR = (extMarginH + inputMenuWidth + interWidth) + wh + newSizeH + interWidth + wh;
        topR = extMarginV + hh;
        size = newSizeH;
    }

    QRect leftCanvasRect(leftL, topL, size, size);
    QRect rightCanvasRect(leftR, topR, size, size);


    if (leftCanvasRect != leftCanvas->geometry() || rightCanvasRect != rightCanvas->geometry())
    {
        leftCanvas->setGeometry(leftCanvasRect);
        leftCanvas->rescale();
        rightCanvas->setGeometry(rightCanvasRect);
        rightCanvas->rescale();
    }
}

int Window::menuWidth() const
{
    return std::max(std::max(inputMenu->maxWidth(), displayMenu->maxWidth()), outputMenu->maxWidth());
}

void Window::optimalSize(uint &outputWidth, uint &outputHeight) const
{
    uint screenWidth = QApplication::desktop()->width();
    uint screenHeight = QApplication::desktop()->height();

    int canvasOptimalSize = std::max(inputMenu->maxHeight() + outputMenu->maxHeight() + displayMenu->maxHeight() + 2*layout->verticalSpacing(),
                                     Tools::intRound((0.9*screenWidth - menuWidth() - 2*layout->margin() - 2*layout->spacing())/2));

    outputHeight = std::max(inputMenu->maxHeight() + outputMenu->maxHeight() + displayMenu->maxHeight()
                            + 2*layout->verticalSpacing(), canvasOptimalSize) + 2*layout->margin() + statusBar->height() + 2*layout->verticalSpacing() + topMenu->sizeHint().height();
    if(outputHeight > screenHeight)
    {
        outputHeight = 0.9*(inputMenu->maxHeight() + outputMenu->maxHeight() + displayMenu->maxHeight()
                            + 2*layout->verticalSpacing() + 2*layout->margin() + statusBar->height() + layout->verticalSpacing() + topMenu->height());
    }
    outputWidth = std::min(menuWidth() + 2*layout->margin() + 2*canvasOptimalSize + 2*layout->horizontalSpacing(), Tools::intRound(0.9*screenWidth));
}


