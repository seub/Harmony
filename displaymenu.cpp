#include "displaymenu.h"

#include <QGridLayout>
#include <QStyle>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

#include "leftmenu.h"

DisplayMenu::DisplayMenu(LeftMenu *leftMenu)
{
    setParent(leftMenu);
    setTitle("Display");

    vertSpace = 3;

    createButtons();
    createLayout();
}

void DisplayMenu::createButtons()
{
    resetViewButton = new QPushButton("Reset view");

    showTranslatesLabel = new QLabel("Show translates: ");

    showTranslatesComboBox = new QComboBox;
    showTranslatesComboBox->addItem("Fund. domain boundary", SHOW_TRANSLATES_DOMAIN);
    showTranslatesComboBox->addItem("Around a vertex", SHOW_TRANSLATES_VERTEX);
    showTranslatesComboBox->addItem("Around all vertices", SHOW_TRANSLATES_VERTICES);
    showTranslatesComboBox->addItem("Around all vertices*", SHOW_TRANSLATES_VERTICES_STAR);
    showTranslatesComboBox->setToolTip("Show mesh translates under the domain representation...");

    coloringLabel = new QLabel("Choose coloring: ");
/*
    coloringComboBox = new QComboBox;
    coloringComboBox->addItem("None", COLORING_NONE);
    coloringComboBox->addItem("Plain", COLORING_PLAIN);
    coloringComboBox->setToolTip("Choose coloring...");
*/

    coloringComboBoxLeft = new QComboBox;
    coloringComboBoxLeft->addItem("None", COLORING_NONE_LEFT);
    coloringComboBoxLeft->addItem("Plain", COLORING_PLAIN_LEFT);
    coloringComboBoxLeft->setToolTip("Choose coloring...");

    coloringComboBoxRight = new QComboBox;
    coloringComboBoxRight->addItem("None", COLORING_NONE_RIGHT);
    coloringComboBoxRight->addItem("Plain", COLORING_PLAIN_RIGHT);
    coloringComboBoxRight->setToolTip("Choose coloring...");

    /*
    colorComboBox = new QComboBox;
    colorComboBox->addItem("Red", RED);
    colorComboBox->addItem("Green", GREEN);
    colorComboBox->addItem("Blue", BLUE);
    colorComboBox->addItem("Light blue", LIGHT_BLUE);
    colorComboBox->addItem("Orange", ORANGE);
    colorComboBox->addItem("Gray", GRAY);
    colorComboBox->addItem("Black", BLACK);
    colorComboBox->setToolTip("Choose color...");
    */

    colorComboBoxLeft = new QComboBox;
    colorComboBoxLeft->addItem("Red", RED_L);
    colorComboBoxLeft->addItem("Green", GREEN_L);
    colorComboBoxLeft->addItem("Blue", BLUE_L);
    colorComboBoxLeft->addItem("Light blue", LIGHT_BLUE_L);
    colorComboBoxLeft->addItem("Orange", ORANGE_L);
    colorComboBoxLeft->addItem("Gray", GRAY_L);
    colorComboBoxLeft->addItem("Black", BLACK_L);
    colorComboBoxLeft->setToolTip("Choose color...");

    colorComboBoxRight = new QComboBox;
    colorComboBoxRight->addItem("Red", RED_R);
    colorComboBoxRight->addItem("Green", GREEN_R);
    colorComboBoxRight->addItem("Blue", BLUE_R);
    colorComboBoxRight->addItem("Light blue", LIGHT_BLUE_R);
    colorComboBoxRight->addItem("Orange", ORANGE_R);
    colorComboBoxRight->addItem("Gray", GRAY_R);
    colorComboBoxRight->addItem("Black", BLACK_R);
    colorComboBoxRight->setToolTip("Choose color...");

    buttonHeight = showTranslatesComboBox->sizeHint().height();
    resetViewButton->setFixedHeight(buttonHeight);
    showTranslatesLabel->setFixedHeight(buttonHeight);
    showTranslatesComboBox->setFixedHeight(buttonHeight);
    coloringLabel->setFixedHeight(buttonHeight);
    //coloringComboBox->setFixedHeight(buttonHeight);
    //colorComboBox->setFixedHeight(buttonHeight);
    coloringComboBoxLeft->setFixedHeight(buttonHeight);
    colorComboBoxLeft->setFixedHeight(buttonHeight);
    coloringComboBoxRight->setFixedHeight(buttonHeight);
    colorComboBoxRight->setFixedHeight(buttonHeight);
}

void DisplayMenu::createLayout()
{
    layout = new QGridLayout;
    layout->setSpacing(0);

    layout->setRowMinimumHeight(0, 2*vertSpace);
    layout->setRowMinimumHeight(1, buttonHeight);
    layout->setRowMinimumHeight(2, 2*vertSpace);
    layout->setRowMinimumHeight(3, buttonHeight);
    layout->setRowMinimumHeight(4, 1*vertSpace);
    layout->setRowMinimumHeight(5, buttonHeight);
    layout->setRowMinimumHeight(6, 2*vertSpace);
    layout->setRowMinimumHeight(7, buttonHeight);
    layout->setRowMinimumHeight(8, 1*vertSpace);
    layout->setRowMinimumHeight(9, buttonHeight);
    layout->setRowMinimumHeight(10, buttonHeight);

    setLayout(layout);

    layout->addWidget(resetViewButton, 1, 0, 1, 2);
    resetViewButton->setVisible(true);
    resetViewButton->setEnabled(true);

    layout->addWidget(showTranslatesLabel, 3, 0, 1, 2);
    showTranslatesLabel->setVisible(true);
    showTranslatesLabel->setEnabled(true);

    layout->addWidget(showTranslatesComboBox, 5, 0, 1, 2);
    showTranslatesComboBox->setVisible(true);
    showTranslatesComboBox->setEnabled(true);

    layout->addWidget(coloringLabel, 7, 0, 1, 2);
    coloringLabel->setVisible(true);
    coloringLabel->setEnabled(true);
/*
    layout->addWidget(coloringComboBox, 9, 0, 1, 1);
    coloringComboBox->setVisible(true);
    coloringComboBox->setEnabled(true);

    layout->addWidget(colorComboBox, 9, 1, 1, 1);
    colorComboBox->setVisible(true);
    colorComboBox->setEnabled(true);
*/
    layout->addWidget(coloringComboBoxLeft, 9, 0, 1, 1);
    coloringComboBoxLeft->setVisible(true);
    coloringComboBoxLeft->setEnabled(true);

    layout->addWidget(colorComboBoxLeft, 9, 1, 1, 1);
    colorComboBoxLeft->setVisible(true);
    colorComboBoxLeft->setEnabled(true);

    layout->addWidget(coloringComboBoxRight, 10, 0, 1, 1);
    coloringComboBoxRight->setVisible(true);
    coloringComboBoxRight->setEnabled(true);

    layout->addWidget(colorComboBoxRight, 10, 1, 1, 1);
    colorComboBoxRight->setVisible(true);
    colorComboBoxRight->setEnabled(true);
}

void DisplayMenu::setReady(bool left)
{
    setEnabled(left);
}
