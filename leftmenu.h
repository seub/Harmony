#ifndef LEFTMENU_H
#define LEFTMENU_H

#include <QWidget>

#include "tools.h"

class QVBoxLayout;

class Window; class InputMenu; class DisplayMenu; class OutputMenu;

class LeftMenu : public QWidget
{
    Q_OBJECT

    friend class Window;
    friend class ActionHandler;

public:
    LeftMenu() = delete;
    LeftMenu(const LeftMenu &) = delete;
    LeftMenu & operator=(LeftMenu) = delete;

private:
    explicit LeftMenu(Window *window);

    void createSubMenus();

    QVBoxLayout *layout;
    InputMenu *inputMenu;
    DisplayMenu *displayMenu;
    OutputMenu *outputMenu;
};

#endif // LEFTMENU_H
