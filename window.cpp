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
#include "h2canvasdelegate.h"



Window::Window(ActionHandler *handler)
{
    createWindow(handler);
}

void Window::createWindow(ActionHandler *handler)
{
    leftCanvas = new Canvas(H2DELEGATEDOMAIN, this, true, false, handler);
    rightCanvas = new Canvas(H2DELEGATETARGET, this, false, true, handler);
    inputMenu = new InputMenu(this);
    outputMenu = new OutputMenu(this, handler);
    statusBar= new QStatusBar(this);
    statusBar->setSizeGripEnabled(false);
    statusBar->setFixedHeight(25);
    statusBarLabel = new QLabel;
    statusBarLabel->setText("Ready");
    statusBar->insertWidget(0, statusBarLabel);
    topMenu = new TopMenu(this);
    layout = new QGridLayout;

    layout->setMargin(9);
    layout->setSpacing(6);
    layout->setMenuBar(topMenu);


    layout->setColumnMinimumWidth(0, menuWidth());
    int leftColMenuSize = std::max(inputMenu->maxLeftColWidth(), outputMenu->maxLeftColWidth());
    inputMenu->layout->setColumnMinimumWidth(0, leftColMenuSize);
    outputMenu->layout->setColumnMinimumWidth(0, leftColMenuSize);


    layout->addWidget(leftCanvas, 0, 1, 2, 1);
    layout->addWidget(rightCanvas, 0, 2, 2, 1);
    layout->addWidget(inputMenu, 0, 0, Qt::AlignTop);
    layout->addWidget(outputMenu, 1, 0, Qt::AlignTop);
    layout->addWidget(statusBar, 2, 0, 1, 3);
    setLayout(layout);


    setWindowTitle(tr("Equivariant Harmonic Maps"));

    //QIcon icon(":/images/icon.png");
    //setWindowIcon(icon);

    unsigned int w, h;
    optimalSize(w,h);
    resize(w, h);

    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

void Window::resizeEvent(QResizeEvent * event)
{
    int delta_width = abs(event->size().width() - event->oldSize().width());
    int delta_height = abs(event->size().height() - event->oldSize().height());

    if (delta_width >= 1 || delta_height >= 1 || leftCanvas->width()!=leftCanvas->height())
    {
        enableCanvasesUpdates(false);
        resizeCanvases();
        enableCanvasesUpdates(true);
    }
}

void Window::enableCanvasesUpdates(bool b)
{
    leftCanvas->setUpdatesEnabled(b);
    rightCanvas->setUpdatesEnabled(b);
}

void Window::resizeCanvases()
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
    return std::max(inputMenu->maxWidth(), outputMenu->maxWidth());
}

void Window::optimalSize(unsigned int &outputWidth, unsigned int &outputHeight) const
{
    unsigned int screenWidth = QApplication::desktop()->width();
    unsigned int screenHeight = QApplication::desktop()->height();

    int canvasOptimalSize = std::max(inputMenu->maxHeight() + outputMenu->maxHeight() +layout->verticalSpacing(),
                                       Tools::intRound((0.9*screenWidth - menuWidth() - 2*layout->margin() - 2*layout->spacing())/2));

    outputHeight = std::max(inputMenu->maxHeight() + outputMenu->maxHeight()
                 +layout->verticalSpacing(),canvasOptimalSize) + 2*layout->margin() + statusBar->height() +layout->verticalSpacing() + topMenu->sizeHint().height();
    if(outputHeight > screenHeight)
    {
        outputHeight = 0.9*(inputMenu->maxHeight() + outputMenu->maxHeight()
                 +layout->verticalSpacing() + 2*layout->margin() + statusBar->height() +layout->verticalSpacing() + topMenu->height());
    }
    outputWidth = std::min(menuWidth() + 2*layout->margin() + 2*canvasOptimalSize + 2*layout->horizontalSpacing(), Tools::intRound(0.9*screenWidth));
}


