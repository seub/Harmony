#include "displaymenu.h"

#include <QGridLayout>
#include <QStyle>
#include <QComboBox>
#include <QLabel>

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
    showTranslatesLabel = new QLabel("Show translates: ");

    showTranslatesComboBox = new QComboBox;
    showTranslatesComboBox->addItem("Fund. domain boundary", SHOW_TRANSLATES_DOMAIN);
    showTranslatesComboBox->addItem("Around a vertex", SHOW_TRANSLATES_VERTEX);
    showTranslatesComboBox->addItem("Around all vertices", SHOW_TRANSLATES_VERTICES);
    showTranslatesComboBox->setToolTip("Show mesh translates under the domain representation...");
    connect(showTranslatesComboBox, SIGNAL(activated(int)), handler, SLOT(showTranslatesClicked(int)));

    buttonHeight = showTranslatesComboBox->sizeHint().height();
    showTranslatesComboBox->setFixedHeight(buttonHeight);
    showTranslatesLabel->setFixedHeight(buttonHeight);
}

void DisplayMenu::createLayout()
{
    layout = new QGridLayout;
    layout->setSpacing(0);

    layout->setRowMinimumHeight(0, 2*vertSpace);
    layout->setRowMinimumHeight(1, buttonHeight);
    layout->setRowMinimumHeight(2, 1*vertSpace);
    layout->setRowMinimumHeight(3, buttonHeight);

    layout->setColumnMinimumWidth(0, maxLeftColWidth());

    setLayout(layout);

    layout->addWidget(showTranslatesLabel, 1, 0, 1, 2);
    showTranslatesLabel->setVisible(true);
    showTranslatesLabel->setEnabled(true);

    layout->addWidget(showTranslatesComboBox, 3, 0, 1, 2);
    showTranslatesComboBox->setVisible(true);
    showTranslatesComboBox->setEnabled(true);
}

void DisplayMenu::setReady(bool left)
{
    setEnabled(left);
}

int DisplayMenu::maxLeftColWidth() const
{
    int leftMax = std::max(0, 0);
    return leftMax;
}

int DisplayMenu::maxRightColWidth() const
{
    int rightMax = std::max(0, 0);
    return rightMax;
}

int DisplayMenu::maxWidth() const
{
    int maxi = maxLeftColWidth() + maxRightColWidth() + layout->horizontalSpacing();
    maxi = std::max(maxi, showTranslatesComboBox->sizeHint().width());
    maxi = std::max(maxi, showTranslatesLabel->sizeHint().width());
    return QStyle::CE_MenuHMargin + maxi + layout->margin();
}


int DisplayMenu::maxHeight() const
{
    int absurdMargin = 1;
    return QStyle::CE_HeaderLabel + absurdMargin + 2*buttonHeight + 3*vertSpace;
}

void DisplayMenu::resizeEvent(QResizeEvent *)
{
    if(height() < maxHeight())
    {
        setMinimumHeight(maxHeight());
    }
}
