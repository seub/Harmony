#include "inputmenu.h"

#include <QGridLayout>
#include <QStyle>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>

#include "actionhandler.h"
#include "window.h"

InputMenu::InputMenu(Window *window, ActionHandler *handler) : handler(handler)
{
    setParent(window);
    setTitle("Input");

    vertSpace = 5;

    createButtons();
    createLayout();
}

void InputMenu::createButtons()
{
    genusLabel = new QLabel("Genus: ");

    genusSpinBox = new QSpinBox();
    genusSpinBox->setRange(2, 7);
    genusSpinBox->setValue(2);
    genusSpinBox->setToolTip("Set genus of the surface");
    connect(genusSpinBox, SIGNAL(valueChanged(int)), handler, SLOT(genusClicked(int)));

    setRhoDomainComboBox = new QComboBox;
    setRhoDomainComboBox->addItem(QString("Set %1 domain...").arg(QChar(961)), SET_RHO_CHOOSE);
    setRhoDomainComboBox->addItem(QString("Token %1nice%2 %3").arg(QChar(8220)).arg(QChar(8221)).arg(QChar(961)), SET_RHO_NICE);
    setRhoDomainComboBox->addItem(QString("Token random %1").arg(QChar(961)), SET_RHO_RANDOM);
    setRhoDomainComboBox->addItem("Fenchel-Nielsen...", SET_RHO_FN);
    setRhoDomainComboBox->setToolTip("Set domain representation...");
    connect(setRhoDomainComboBox, SIGNAL(activated(int)), handler, SLOT(setRhoDomainClicked(int)));

    setRhoImageComboBox = new QComboBox;
    setRhoImageComboBox->addItem(QString("Set %1 image...").arg(QChar(961)), SET_RHO_CHOOSE);
    setRhoImageComboBox->addItem(QString("Token %1nice%2 %3").arg(QChar(8220)).arg(QChar(8221)).arg(QChar(961)), SET_RHO_NICE);
    setRhoImageComboBox->addItem(QString("Token random %1").arg(QChar(961)), SET_RHO_RANDOM);
    setRhoImageComboBox->addItem("Fenchel-Nielsen...", SET_RHO_FN);
    setRhoImageComboBox->setToolTip("Set image representation...");
    connect(setRhoImageComboBox, SIGNAL(activated(int)), handler, SLOT(setRhoImageClicked(int)));

    meshDepthLabel = new QLabel("Mesh depth: ");

    meshDepthSpinBox = new QSpinBox();
    meshDepthSpinBox->setRange(0, 7);
    meshDepthSpinBox->setValue(3);
    meshDepthSpinBox->setToolTip("Choose depth of the mesh");
    connect(meshDepthSpinBox, SIGNAL(valueChanged(int)), handler, SLOT(meshDepthClicked(int)));

    buttonHeight = setRhoDomainComboBox->sizeHint().height();
    genusLabel->setFixedHeight(buttonHeight);
    genusSpinBox->setFixedHeight(buttonHeight);
    setRhoDomainComboBox->setFixedHeight(buttonHeight);
    setRhoImageComboBox->setFixedHeight(buttonHeight);
    meshDepthLabel->setFixedHeight(buttonHeight);
    meshDepthSpinBox->setFixedHeight(buttonHeight);

}

void InputMenu::resetRhos()
{
    setRhoDomainComboBox->setCurrentIndex(SET_RHO_CHOOSE);
    setRhoImageComboBox->setCurrentIndex(SET_RHO_CHOOSE);
}

void InputMenu::createLayout()
{
    layout = new QGridLayout;
    layout->setSpacing(0);



    layout->setRowMinimumHeight(0, 2*vertSpace);
    layout->setRowMinimumHeight(1, buttonHeight);
    layout->setRowMinimumHeight(2, 2*vertSpace);
    layout->setRowMinimumHeight(3, buttonHeight);
    layout->setRowMinimumHeight(4, 2*vertSpace);
    layout->setRowMinimumHeight(5, buttonHeight);
    layout->setRowMinimumHeight(6, 4*vertSpace);
    layout->setRowMinimumHeight(7, buttonHeight);

    layout->setColumnMinimumWidth(0, maxLeftColWidth());

    setLayout(layout);

    layout->addWidget(genusLabel, 1, 0, 1, 1, Qt::AlignRight);
    genusLabel->setVisible(true);
    genusLabel->setEnabled(true);

    layout->addWidget(genusSpinBox, 1, 1, 1, 1);
    genusSpinBox->setVisible(true);
    genusSpinBox->setEnabled(true);

    layout->addWidget(setRhoDomainComboBox, 3, 0, 1, 2);
    setRhoDomainComboBox->setVisible(true);
    setRhoDomainComboBox->setEnabled(true);

    layout->addWidget(setRhoImageComboBox, 5, 0, 1, 2);
    setRhoImageComboBox->setVisible(true);
    setRhoImageComboBox->setEnabled(true);

    layout->addWidget(meshDepthLabel, 7, 0, 1, 1, Qt::AlignRight);
    meshDepthLabel->setVisible(true);
    meshDepthSpinBox->setEnabled(true);

    layout->addWidget(meshDepthSpinBox, 7, 1, 1, 1);
    meshDepthSpinBox->setVisible(true);
    meshDepthSpinBox->setEnabled(true);

}

int InputMenu::maxLeftColWidth() const
{
    int leftMax = std::max(genusLabel->sizeHint().width(), meshDepthLabel->sizeHint().width());
    return leftMax;
}

int InputMenu::maxRightColWidth() const
{
    int rightMax = std::max(genusSpinBox->sizeHint().width(), genusSpinBox->sizeHint().width());
    return rightMax;
}

int InputMenu::maxWidth() const
{
    int maxi = maxLeftColWidth() + maxRightColWidth() + layout->horizontalSpacing();
    maxi = std::max(maxi, setRhoDomainComboBox->sizeHint().width());
    maxi = std::max(maxi, setRhoImageComboBox->sizeHint().width());
    return QStyle::CE_MenuHMargin + maxi + layout->margin();
}


int InputMenu::maxHeight() const
{
    int absurdMargin = 1;
    return QStyle::CE_HeaderLabel + absurdMargin + 4*buttonHeight + 10*vertSpace;
}

int InputMenu::getGenus() const
{
    return genusSpinBox->value();
}

int InputMenu::getMeshDepth() const
{
    return meshDepthSpinBox->value();
}

void InputMenu::resizeEvent(QResizeEvent *)
{
    if(height() < maxHeight())
    {
        setMinimumHeight(maxHeight());
    }
}
