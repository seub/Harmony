#include "outputmenu.h"

#include <QGridLayout>
#include <QStyle>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>

#include "window.h"
#include "actionhandler.h"

OutputMenu::OutputMenu(Window *window, ActionHandler* handler) : handler(handler)
{
    setParent(window);
    setTitle("Discrete heat flow");

    vertSpace = 5;

    createButtons();
    createLayout();
}

void OutputMenu::createButtons()
{
    computeButton = new QPushButton(QString("Run flow (%1)").arg(QChar(0x221E)));
    connect(computeButton, SIGNAL(clicked()), this->handler, SLOT(computeButtonClicked()));
    computeButton->setToolTip("Iterate the discrete heat flow");

    iterateButton = new QPushButton(QString("Iterate flow (N)"));
    connect(iterateButton, SIGNAL(clicked()), this->handler, SLOT(iterateButtonClicked()));
    iterateButton->setToolTip("Iterate the discrete heat flow N times");

    resetButton = new QPushButton(QString("Reset"));
    connect(resetButton, SIGNAL(clicked()), this->handler, SLOT(outputResetButtonClicked()));
    resetButton->setToolTip("Reset the function to its initialization");

    showLiveLabel = new QLabel("Show live ");
    showLiveCheckbox = new QCheckBox;
    showLiveCheckbox->setToolTip("Show function while running the discrete heat flow");

    nbIterationsLabel = new QLabel("N = ");
    nbIterationsSpinBox = new QSpinBox();
    nbIterationsSpinBox->setRange(0, 1000);
    nbIterationsSpinBox->setValue(20);
    nbIterationsSpinBox->setToolTip("Choose N for number of iterations");


    buttonHeight = computeButton->sizeHint().height();
    computeButton->setFixedHeight(buttonHeight);
    iterateButton->setFixedHeight(buttonHeight);
    resetButton->setFixedHeight(buttonHeight);
    showLiveLabel->setFixedHeight(buttonHeight);
    showLiveCheckbox->setFixedHeight(buttonHeight);
    nbIterationsLabel->setFixedHeight(buttonHeight);
    nbIterationsSpinBox->setFixedHeight(buttonHeight);
}

void OutputMenu::createLayout()
{
    layout = new QGridLayout;
    layout->setSpacing(0);

    layout->setRowMinimumHeight(0, 2*vertSpace);
    layout->setRowMinimumHeight(1, buttonHeight);
    layout->setRowMinimumHeight(2, 4*vertSpace);
    layout->setRowMinimumHeight(3, buttonHeight);
    layout->setRowMinimumHeight(4, vertSpace);
    layout->setRowMinimumHeight(5, buttonHeight);
    layout->setRowMinimumHeight(6, 4*vertSpace);
    layout->setRowMinimumHeight(7, buttonHeight);
    layout->setRowMinimumHeight(8, vertSpace);
    layout->setRowMinimumHeight(9, buttonHeight);



    layout->setColumnMinimumWidth(0, maxLeftColWidth());

    setLayout(layout);

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
    showLiveCheckbox->setChecked(true);

    layout->addWidget(iterateButton, 7, 0, 1, 2);
    iterateButton->setVisible(true);
    iterateButton->setEnabled(true);

    layout->addWidget(nbIterationsLabel, 9, 0, 1, 1, Qt::AlignRight);
    nbIterationsLabel->setVisible(true);
    nbIterationsLabel->setEnabled(true);

    layout->addWidget(nbIterationsSpinBox, 9, 1, 1, 1);
    nbIterationsSpinBox->setVisible(true);
    nbIterationsSpinBox->setEnabled(true);
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
    //showLiveCheckbox->setChecked(false);

    nbIterationsLabel->setEnabled(true);
    nbIterationsSpinBox->setEnabled(true);
}

void OutputMenu::disableAllButStop()
{
    resetButton->setEnabled(false);
    iterateButton->setEnabled(false);
    showLiveLabel->setEnabled(false);
    showLiveCheckbox->setEnabled(false);
    nbIterationsLabel->setEnabled(false);
    nbIterationsSpinBox->setEnabled(false);
}

void OutputMenu::enableAll()
{
    resetButton->setEnabled(true);
    iterateButton->setEnabled(true);
    showLiveLabel->setEnabled(true);
    showLiveCheckbox->setEnabled(true);
    nbIterationsLabel->setEnabled(true);
    nbIterationsSpinBox->setEnabled(true);
}

void OutputMenu::switchComputeToStopButton()
{
    computeButton->setText(tr("Stop"));
    disconnect(computeButton, SIGNAL(clicked()), handler, SLOT(computeButtonClicked()));
    connect(computeButton, SIGNAL(clicked()), handler, SLOT(stopButtonClicked()));
}

void OutputMenu::switchStopToComputeButton()
{
    computeButton->setText(QString("Run flow (%1)").arg(QChar(0x221E)));
    disconnect(computeButton, SIGNAL(clicked()), handler, SLOT(stopButtonClicked()));
    connect(computeButton, SIGNAL(clicked()), handler, SLOT(computeButtonClicked()));
}

void OutputMenu::enableReset()
{
    resetButton->setEnabled(true);
}

int OutputMenu::maxLeftColWidth() const
{
    int leftMax = std::max(showLiveLabel->sizeHint().width(), nbIterationsLabel->sizeHint().width());

    return leftMax;
}

int OutputMenu::maxRightColWidth() const
{
    int rightMax = std::max(showLiveCheckbox->sizeHint().width(), nbIterationsSpinBox->sizeHint().width());

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
    return QStyle::CE_HeaderLabel + 5*buttonHeight + 12*vertSpace + absurdMargin;
}

int OutputMenu::getNbIterations() const
{
   return nbIterationsSpinBox->value();
}
