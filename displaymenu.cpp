#include "displaymenu.h"

#include <QGridLayout>
#include <QStyle>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

#include "actionhandler.h"
#include "window.h"

DisplayMenu::DisplayMenu(Window *window, ActionHandler *handler) : handler(handler)
{
    setParent(window);
    setTitle("Display");

    vertSpace = 5;

    createButtons();
    createLayout();
}

void DisplayMenu::createButtons()
{
    resetViewButton = new QPushButton("Reset view");
    connect(resetViewButton, SIGNAL(clicked()), handler, SLOT(resetViewButtonClicked()));

    showTranslatesLabel = new QLabel("Show translates: ");

    showTranslatesComboBox = new QComboBox;
    showTranslatesComboBox->addItem("Fund. domain boundary", SHOW_TRANSLATES_DOMAIN);
    showTranslatesComboBox->addItem("Around a vertex", SHOW_TRANSLATES_VERTEX);
    showTranslatesComboBox->addItem("Around all vertices", SHOW_TRANSLATES_VERTICES);
    showTranslatesComboBox->setToolTip("Show mesh translates under the domain representation...");
    connect(showTranslatesComboBox, SIGNAL(activated(int)), handler, SLOT(showTranslatesClicked(int)));

    coloringLabel = new QLabel("Choose coloring: ");

    coloringComboBox = new QComboBox;
    coloringComboBox->addItem("None", COLORING_NONE);
    coloringComboBox->addItem("Plain", COLORING_PLAIN);
    coloringComboBox->addItem("Gradient", COLORING_GRADIENT);
    coloringComboBox->setToolTip("Choose coloring...");
    connect(coloringComboBox, SIGNAL(activated(int)), handler, SLOT(coloringClicked(int)));

    colorComboBox = new QComboBox;
    colorComboBox->addItem("Red", RED);
    colorComboBox->addItem("Green", GREEN);
    colorComboBox->addItem("Blue", BLUE);
    colorComboBox->addItem("Light blue", LIGHT_BLUE);
    colorComboBox->addItem("Orange", ORANGE);
    colorComboBox->addItem("Grey", GREY);
    colorComboBox->addItem("Black", BLACK);
    colorComboBox->setToolTip("Choose color...");
    connect(colorComboBox, SIGNAL(activated(int)), handler, SLOT(colorClicked(int)));

    buttonHeight = showTranslatesComboBox->sizeHint().height();
    resetViewButton->setFixedHeight(buttonHeight);
    showTranslatesLabel->setFixedHeight(buttonHeight);
    showTranslatesComboBox->setFixedHeight(buttonHeight);
    coloringLabel->setFixedHeight(buttonHeight);
    coloringComboBox->setFixedHeight(buttonHeight);
    colorComboBox->setFixedHeight(buttonHeight);
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

    layout->setColumnMinimumWidth(0, maxLeftColWidth());
    layout->setColumnMinimumWidth(1, maxRightColWidth());

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

    layout->addWidget(coloringComboBox, 9, 0, 1, 1);
    coloringComboBox->setVisible(true);
    coloringComboBox->setEnabled(true);

    layout->addWidget(colorComboBox, 9, 1, 1, 1);
    colorComboBox->setVisible(true);
    colorComboBox->setEnabled(true);
}

void DisplayMenu::setReady(bool left)
{
    setEnabled(left);
}

int DisplayMenu::maxLeftColWidth() const
{
    int leftMax = std::max(coloringComboBox->sizeHint().width(), 0);
    return leftMax;
}

int DisplayMenu::maxRightColWidth() const
{
    int rightMax = std::max(colorComboBox->sizeHint().width(), 0);
    return rightMax;
}

int DisplayMenu::maxWidth() const
{
    int maxi = maxLeftColWidth() + maxRightColWidth() + layout->horizontalSpacing();
    maxi = std::max(maxi, showTranslatesComboBox->sizeHint().width());
    maxi = std::max(maxi, showTranslatesLabel->sizeHint().width());
    maxi = std::max(maxi, coloringLabel->sizeHint().width());
    return QStyle::CE_MenuHMargin + maxi + layout->margin();
}


int DisplayMenu::maxHeight() const
{
    int absurdMargin = 1;
    return QStyle::CE_HeaderLabel + absurdMargin + 5*buttonHeight + 8*vertSpace;
}

void DisplayMenu::resizeEvent(QResizeEvent *)
{
    if(height() < maxHeight())
    {
        setMinimumHeight(maxHeight());
    }
}
