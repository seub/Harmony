#include "inputmenu.h"

#include <QGridLayout>
#include <QStyle>
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
    showTranslatesComboBox = new QComboBox;
    showTranslatesComboBox->addItem("Show translates...", SHOW_TRANSLATES_CHOOSE);
    showTranslatesComboBox->addItem("Domain boundary", SHOW_TRANSLATES_DOMAIN);
    showTranslatesComboBox->addItem("Around a vertex", SHOW_TRANSLATES_VERTEX);
    showTranslatesComboBox->addItem("Around all vertices", SHOW_TRANSLATES_VERTICES);
    showTranslatesComboBox->setToolTip("Show translates under the representation...");
    connect(showTranslatesComboBox, SIGNAL(activated(int)), handler, SLOT(inputShowTranslatesChoice(int)));

    buttonHeight = showTranslatesComboBox->sizeHint().height();
    showTranslatesComboBox->setFixedHeight(buttonHeight);
}

void InputMenu::createLayout()
{
    layout = new QGridLayout;
    layout->setSpacing(0);

    layout->setRowMinimumHeight(0, vertSpace);
    layout->setRowMinimumHeight(1, buttonHeight);

    layout->setColumnMinimumWidth(0, maxLeftColWidth());

    setLayout(layout);

    layout->addWidget(showTranslatesComboBox, 1, 0, 1, 2);
    showTranslatesComboBox->setVisible(true);
    showTranslatesComboBox->setEnabled(true);
}

int InputMenu::maxLeftColWidth() const
{
    int leftMax = std::max(0, 0);
    return leftMax;
}

int InputMenu::maxRightColWidth() const
{
    int rightMax = std::max(0, 0);
    return rightMax;
}

int InputMenu::maxWidth() const
{
    int maxi = maxLeftColWidth() + maxRightColWidth() + layout->horizontalSpacing();
    maxi = std::max(maxi, showTranslatesComboBox->sizeHint().width());
    return QStyle::CE_MenuHMargin + maxi + layout->margin();
}


int InputMenu::maxHeight() const
{
    int absurdMargin = 1;
    return QStyle::CE_HeaderLabel + absurdMargin + 1*buttonHeight + 1*vertSpace;
}

void InputMenu::resizeEvent(QResizeEvent *)
{
    if(height() < maxHeight())
    {
        setMinimumHeight(maxHeight());
    }
}
