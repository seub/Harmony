#include "outputmenu.h"

#include <QGridLayout>
#include <QStyle>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

#include "window.h"
#include "actionhandler.h"

OutputMenu::OutputMenu(Window * const window, ActionHandler* handler) : handler(handler)
{
    setParent(window);
    setTitle(tr("Output"));

    computeButton = new QPushButton(QString("Run discrete flow (%1)").arg(QChar(0x221E)));
    connect(computeButton, SIGNAL(clicked()), this->handler, SLOT(computeButtonClicked()));
    iterateButton = new QPushButton(QString("Run discrete flow (N)"));
    connect(iterateButton, SIGNAL(clicked()), this->handler, SLOT(iterateButtonClicked()));
    resetButton = new QPushButton(QString("Reset"));
    connect(resetButton, SIGNAL(clicked()), this->handler, SLOT(outputResetButtonClicked()));


    vertSpace = 5;
    buttonHeight = computeButton->sizeHint().height();
    computeButton->setFixedHeight(buttonHeight);
    iterateButton->setFixedHeight(buttonHeight);
    resetButton->setFixedHeight(buttonHeight);

    layout = new QGridLayout;
    layout->setSpacing(0);

    layout->setRowMinimumHeight(0, vertSpace);
    layout->setRowMinimumHeight(1, buttonHeight);
    layout->setRowMinimumHeight(2, 4*vertSpace);
    layout->setRowMinimumHeight(3, buttonHeight);
    layout->setRowMinimumHeight(4, vertSpace);
    layout->setRowMinimumHeight(5, buttonHeight);
    layout->setRowMinimumHeight(6, 4*vertSpace);
    layout->setRowMinimumHeight(7, buttonHeight);

    showLiveLabel = new QLabel("Show live ");
    showLiveCheckbox = new QCheckBox;
    showLiveLabel->setFixedHeight(buttonHeight);
    showLiveCheckbox->setFixedHeight(buttonHeight);

    layout->setColumnMinimumWidth(0,maxLeftColWidth());

    setLayout(layout);
    createStandardMenu();
}

void OutputMenu::createStandardMenu()
{
    layout->addWidget(resetButton, 1, 0, 1, 2);
    resetButton->setVisible(true);
    resetButton->setEnabled(false);

    layout->addWidget(computeButton, 3, 0, 1, 2);
    computeButton->setVisible(true);
    computeButton->setEnabled(true);

    layout->addWidget(showLiveLabel, 5, 0, 1, 1, Qt::AlignRight);
    showLiveLabel->setVisible(true);
    layout->addWidget(showLiveCheckbox, 5, 1, 1, 1);
    showLiveCheckbox->setVisible(true);
    showLiveCheckbox->setChecked(false);

    layout->addWidget(iterateButton, 7, 0, 1, 2);
    iterateButton->setVisible(true);
    iterateButton->setEnabled(true);
}

void OutputMenu::resizeEvent(QResizeEvent *)
{
    if(height() < maxHeight())
    {
        setMinimumHeight(maxHeight());
    }
}

void OutputMenu::resetMenu()
{
    resetButton->setEnabled(false);
    computeButton->setEnabled(true);
    iterateButton->setEnabled(true);
    showLiveLabel->setEnabled(true);

    showLiveCheckbox->setEnabled(true);
    showLiveCheckbox->setChecked(false);
}

void OutputMenu::disableAllButStop()
{
    resetButton->setEnabled(false);
    iterateButton->setEnabled(false);
    showLiveLabel->setEnabled(false);
    showLiveCheckbox->setEnabled(false);
}

void OutputMenu::enableReset()
{
    resetButton->setEnabled(true);
}

int OutputMenu::maxLeftColWidth() const
{
    int leftMax = showLiveLabel->sizeHint().width();

    return leftMax;
}

int OutputMenu::maxRightColWidth() const
{
    int rightMax = showLiveCheckbox->sizeHint().width();

    return rightMax;
}

int OutputMenu::maxWidth() const
{
    int maxi = maxLeftColWidth() + maxRightColWidth() + layout->horizontalSpacing();
    maxi = std::max(maxi, computeButton->sizeHint().width());
    maxi = std::max(maxi, iterateButton->sizeHint().width());
    maxi = std::max(maxi, resetButton->sizeHint().width());
    return maxi + layout->margin() + QStyle::CE_MenuHMargin;
}

int OutputMenu::maxHeight() const
{
    int absurdMargin = 1;
    return QStyle::CE_HeaderLabel + 4*buttonHeight + 10*vertSpace + absurdMargin;
}

void OutputMenu::enterEvent(QEvent *)
{
}
