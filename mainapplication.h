#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QApplication>

#include "tools.h"
#include "mathscontainer.h"
#include "actionhandler.h"

class Window;

class MainApplication : public QApplication
{
public:
    MainApplication(int& argc, char ** argv);

    MainApplication() = delete;
    MainApplication(const MainApplication &) = delete;
    MainApplication & operator=(MainApplication) = delete;

    virtual bool notify(QObject *receiver, QEvent *sender);

private:
    void createWindow();

    Window *window;
    bool errorCaught;
    MathsContainer container;
    ActionHandler handler;
};

#endif // MAINAPPLICATION_H
