#ifndef TOPMENU_H
#define TOPMENU_H

#include <QMenuBar>

class Window;

class TopMenu : public QMenuBar
{
    Q_OBJECT

    friend class Window;

public:
    explicit TopMenu(Window *window);

signals:

public slots:


private:
    Window* window;
};

#endif // TOPMENU_H
