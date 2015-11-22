#include "outputmenu.h"

#include <QGridLayout>
#include <QStyle>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>

#include "leftmenu.h"

OutputMenu::OutputMenu(LeftMenu *leftMenu)
{
    setParent(leftMenu);
    setTitle("Discrete heat flow");

    vertSpace = 5;

    createButtons();
    createLayout();
}

void OutputMenu::createButtons()
{
    iterateButton = new QPushButton(QString("Run flow (N)"));
    iterateButton->setToolTip("Iterate the discrete heat flow");

    computeButton = new QPushButton(QString("Run flow (%1)").arg(QChar(0x221E)));
    computeButton->setToolTip("Run the discrete heat flow");

    resetButton = new QPushButton(QString("Reset"));
    resetButton->setToolTip("Reset the function to its initialization");


    iterateLabel = new QLabel("N = ");
    iterateSpinBox = new QSpinBox();
    iterateSpinBox->setRange(1, 10000);
    iterateSpinBox->setValue(1);
    iterateSpinBox->setToolTip("Choose number of iterations");

    showLiveLabel = new QLabel("Show live ");
    showLiveCheckbox = new QCheckBox;
    showLiveCheckbox->setToolTip("Show function while running the discrete heat flow");

    buttonHeight = computeButton->sizeHint().height();
    iterateButton->setFixedHeight(buttonHeight);
    computeButton->setFixedHeight(buttonHeight);
    resetButton->setFixedHeight(buttonHeight);
    showLiveLabel->setFixedHeight(buttonHeight);
    showLiveCheckbox->setFixedHeight(buttonHeight);
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

    layout->addWidget(resetButton, 1, 0, 1, 2);
    resetButton->setVisible(true);
    resetButton->setEnabled(false);

    layout->addWidget(iterateButton, 3, 0, 1, 2);
    iterateButton->setVisible(true);
    iterateButton->setEnabled(true);

    layout->addWidget(iterateLabel, 5, 0, 1, 1, Qt::AlignRight);
    showLiveLabel->setVisible(true);
    layout->addWidget(iterateSpinBox, 5, 1, 1, 1);
    showLiveCheckbox->setVisible(true);


    layout->addWidget(computeButton, 7, 0, 1, 2);
    computeButton->setVisible(true);
    computeButton->setEnabled(true);

    layout->addWidget(showLiveLabel, 9, 0, 1, 1, Qt::AlignRight);
    showLiveLabel->setVisible(true);
    layout->addWidget(showLiveCheckbox, 9, 1, 1, 1);
    showLiveCheckbox->setVisible(true);
    showLiveCheckbox->setChecked(true);

    setLayout(layout);
}

void OutputMenu::resetMenu()
{
    resetButton->setEnabled(false);

    iterateButton->setEnabled(true);
    iterateLabel->setEnabled(true);
    iterateSpinBox->setEnabled(true);

    computeButton->setEnabled(true);    
    showLiveLabel->setEnabled(true);
    showLiveCheckbox->setEnabled(true);
    //showLiveCheckbox->setChecked(false);
}

void OutputMenu::disableAllButStop()
{
    resetButton->setEnabled(false);

    iterateButton->setEnabled(false);
    iterateLabel->setEnabled(false);
    iterateSpinBox->setEnabled(false);

    showLiveLabel->setEnabled(false);
    showLiveCheckbox->setEnabled(false);
}

void OutputMenu::enableAll()
{
    resetButton->setEnabled(true);
    iterateButton->setEnabled(true);
    iterateLabel->setEnabled(true);
    iterateSpinBox->setEnabled(true);
    showLiveLabel->setEnabled(true);
    showLiveCheckbox->setEnabled(true);
}

void OutputMenu::switchComputeToStopButton()
{
    computeButton->setText(tr("Stop"));
}

void OutputMenu::switchStopToComputeButton()
{
    computeButton->setText(QString("Run flow (%1)").arg(QChar(0x221E)));
}

void OutputMenu::enableReset()
{
    resetButton->setEnabled(true);
}
