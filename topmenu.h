#ifndef TOPMENU_H
#define TOPMENU_H

#include <QMenuBar>

class MainWindow;

class TopMenu : public QMenuBar
{
    Q_OBJECT

    friend class MainWindow;

public:
    explicit TopMenu(MainWindow *window);
    TopMenu() = delete;
    TopMenu(const TopMenu &) = delete;
    TopMenu & operator=(TopMenu) = delete;

signals:

public slots:


private:
    MainWindow* window;
};

#endif // TOPMENU_H
