#include "inputmenu.h"

#include <QGridLayout>
#include <QStyle>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>

#include "leftmenu.h"

InputMenu::InputMenu(LeftMenu *leftMenu)
{
    setParent(leftMenu);
    setTitle("Input");

    vertSpace = leftMenu->getVertSpace();

    createButtons();
    createLayout();
}

void InputMenu::createButtons()
{
    genusLabel = new QLabel("Genus: ");

    genusSpinBox = new QSpinBox();
    genusSpinBox->setRange(2, 6);
    genusSpinBox->setValue(2);
    genusSpinBox->setToolTip("Set genus of the surface");

    setRhoDomainComboBox = new QComboBox;
    setRhoDomainComboBox->addItem(QString("Set %1 left...").arg(QChar(961)), SET_RHO_CHOOSE);
    setRhoDomainComboBox->addItem(QString("Token random %1").arg(QChar(961)), SET_RHO_RANDOM);
    setRhoDomainComboBox->addItem(QString("Token %1nice%2 %3").arg(QChar(8220)).arg(QChar(8221)).arg(QChar(961)), SET_RHO_NICE);
    setRhoDomainComboBox->addItem("Fenchel-Nielsen...", SET_RHO_FN);
    setRhoDomainComboBox->setToolTip("Set domain representation...");

    setRhoImageComboBox = new QComboBox;
    setRhoImageComboBox->addItem(QString("Set %1 right...").arg(QChar(961)), SET_RHO_CHOOSE);
    setRhoImageComboBox->addItem(QString("Token random %1").arg(QChar(961)), SET_RHO_RANDOM);
    setRhoImageComboBox->addItem(QString("Token %1nice%2 %3").arg(QChar(8220)).arg(QChar(8221)).arg(QChar(961)), SET_RHO_NICE);
    setRhoImageComboBox->addItem("Fenchel-Nielsen...", SET_RHO_FN);
    setRhoImageComboBox->setToolTip("Set image representation...");

    meshDepthLabel = new QLabel("Mesh depth: ");

    meshDepthSpinBox = new QSpinBox();
    meshDepthSpinBox->setRange(0, 6);
    meshDepthSpinBox->setValue(4);
    meshDepthSpinBox->setToolTip("Choose depth of the mesh");

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

    setLayout(layout);
}

int InputMenu::getGenus() const
{
    return genusSpinBox->value();
}

int InputMenu::getMeshDepth() const
{
    return meshDepthSpinBox->value();
}
