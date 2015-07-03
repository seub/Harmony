#include "topmenu.h"

#include "mainwindow.h"

TopMenu::TopMenu(MainWindow * window) : window(window)
{
    setParent(window);
}
