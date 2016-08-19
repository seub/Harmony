#ifndef LEFTMENU_H
#define LEFTMENU_H

#include <QWidget>

#include "tools.h"

class QVBoxLayout;

class MainWindow; class InputMenu; class DisplayMenu; class OutputMenu;

class LeftMenu : public QWidget
{
    Q_OBJECT

    friend class MainWindow;
    friend class ActionHandler;

public:
    LeftMenu() = delete;
    LeftMenu(const LeftMenu &) = delete;
    LeftMenu & operator=(LeftMenu) = delete;

    int getVertSpace() const {return vertSpace;}

private:
    explicit LeftMenu(MainWindow *window);

    void createSubMenus();

    QVBoxLayout *layout;
    InputMenu *inputMenu;
    DisplayMenu *displayMenu;
    OutputMenu *outputMenu;

    int vertSpace;
};

#endif // LEFTMENU_H
