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
    domainLabel = new QLabel("Domain:");

    showTranslatesDomainComboBox = new QComboBox;
    showTranslatesDomainComboBox->addItem("Show translates...", SHOW_TRANSLATES_CHOOSE);
    showTranslatesDomainComboBox->addItem("Fund. domain boundary", SHOW_TRANSLATES_DOMAIN);
    showTranslatesDomainComboBox->addItem("Around a vertex", SHOW_TRANSLATES_VERTEX);
    showTranslatesDomainComboBox->addItem("Around all vertices", SHOW_TRANSLATES_VERTICES);
    showTranslatesDomainComboBox->setToolTip("Show mesh translates under the domain representation...");
    connect(showTranslatesDomainComboBox, SIGNAL(activated(int)), handler, SLOT(domainShowTranslatesClicked(int)));

    imageLabel = new QLabel("Image:");

    showTranslatesImageComboBox = new QComboBox;
    showTranslatesImageComboBox->addItem("Show translates...", SHOW_TRANSLATES_CHOOSE);
    showTranslatesImageComboBox->addItem("Fund. domain boundary", SHOW_TRANSLATES_DOMAIN);
    showTranslatesImageComboBox->addItem("Around a vertex", SHOW_TRANSLATES_VERTEX);
    showTranslatesImageComboBox->addItem("Around all vertices", SHOW_TRANSLATES_VERTICES);
    showTranslatesImageComboBox->setToolTip("Show function translates under the image representation...");
    connect(showTranslatesImageComboBox, SIGNAL(activated(int)), handler, SLOT(imageShowTranslatesClicked(int)));

    buttonHeight = showTranslatesDomainComboBox->sizeHint().height();
    showTranslatesDomainComboBox->setFixedHeight(buttonHeight);
    showTranslatesImageComboBox->setFixedHeight(buttonHeight);
    domainLabel->setFixedHeight(buttonHeight);
    imageLabel->setFixedHeight(buttonHeight);
}

void DisplayMenu::createLayout()
{
    layout = new QGridLayout;
    layout->setSpacing(0);

    layout->setRowMinimumHeight(0, 2*vertSpace);
    layout->setRowMinimumHeight(1, buttonHeight);
    layout->setRowMinimumHeight(2, 1*vertSpace);
    layout->setRowMinimumHeight(3, buttonHeight);
    layout->setRowMinimumHeight(4, 2*vertSpace);
    layout->setRowMinimumHeight(5, buttonHeight);
    layout->setRowMinimumHeight(6, 1*vertSpace);
    layout->setRowMinimumHeight(7, buttonHeight);

    layout->setColumnMinimumWidth(0, maxLeftColWidth());

    setLayout(layout);

    layout->addWidget(domainLabel, 1, 0, 1, 2);
    domainLabel->setVisible(true);
    domainLabel->setEnabled(true);

    layout->addWidget(showTranslatesDomainComboBox, 3, 0, 1, 2);
    showTranslatesDomainComboBox->setVisible(true);
    showTranslatesDomainComboBox->setEnabled(true);

    layout->addWidget(imageLabel, 5, 0, 1, 2);
    imageLabel->setVisible(true);
    imageLabel->setEnabled(true);

    layout->addWidget(showTranslatesImageComboBox, 7, 0, 1, 2);
    showTranslatesImageComboBox->setVisible(true);
    showTranslatesImageComboBox->setEnabled(true);
}

void DisplayMenu::setReady(bool left, bool right)
{
    setEnabled(left);
    if (left)
    {
        imageLabel->setEnabled(right);
        showTranslatesImageComboBox->setEnabled(right);
    }
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
    maxi = std::max(maxi, showTranslatesDomainComboBox->sizeHint().width());
    maxi = std::max(maxi, showTranslatesImageComboBox->sizeHint().width());
    maxi = std::max(maxi, domainLabel->sizeHint().width());
    maxi = std::max(maxi, imageLabel->sizeHint().width());
    return QStyle::CE_MenuHMargin + maxi + layout->margin();
}


int DisplayMenu::maxHeight() const
{
    int absurdMargin = 1;
    return QStyle::CE_HeaderLabel + absurdMargin + 4*buttonHeight + 6*vertSpace;
}

void DisplayMenu::resizeEvent(QResizeEvent *)
{
    if(height() < maxHeight())
    {
        setMinimumHeight(maxHeight());
    }
}
