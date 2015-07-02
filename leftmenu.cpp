#include "leftmenu.h"

#include <QVBoxLayout>

#include "window.h"
#include "inputmenu.h"
#include "displaymenu.h"
#include "outputmenu.h"

LeftMenu::LeftMenu(Window *window)
{
    setParent(window);
    createSubMenus();
}

void LeftMenu::createSubMenus()
{
    inputMenu = new InputMenu(this);
    inputMenu->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    inputMenu->setEnabled(true);

    outputMenu = new OutputMenu(this);
    outputMenu->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    outputMenu->setEnabled(false);

    displayMenu = new DisplayMenu(this);
    displayMenu->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    displayMenu->setEnabled(false);


    layout = new QVBoxLayout;
    layout->setMargin(3);
    layout->setSpacing(10);

    layout->addWidget(inputMenu, Qt::AlignCenter);
    layout->addWidget(outputMenu, Qt::AlignCenter);
    layout->addWidget(displayMenu, Qt::AlignCenter);

    setLayout(layout);
}
