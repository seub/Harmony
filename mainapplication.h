#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QApplication>

#include "tools.h"
#include "mathscontainer.h"
#include "actionhandler.h"
#include "topfactory.h"
#include "mainwindow.h"

class MainWindow;

class MainApplication : public QApplication
{
public:
    MainApplication(int& argc, char ** argv);

    MainApplication() = delete;
    MainApplication(const MainApplication &) = delete;
    MainApplication & operator=(MainApplication) = delete;

    virtual bool notify(QObject *receiver, QEvent *sender);

private:
    MainWindow *window;
    bool errorCaught;
    MathsContainer mathsContainer;
    ActionHandler handler;
    std::unique_ptr<TopFactory> factory;
};

#endif // MAINAPPLICATION_H
