#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "tools.h"

class QGridLayout;

class Canvas; class LeftMenu; class TopMenu;
class ActionHandler; class StatusBar;

class MainWindow : public QWidget
{
    Q_OBJECT

    friend class ActionHandler;
    friend class MainApplication;
    friend class Canvas;

public:
    MainWindow(ActionHandler *handler = nullptr);
    MainWindow(const MainWindow &) = delete;
    MainWindow & operator =(MainWindow) = delete;

private:
    void createWindow(ActionHandler *handler = nullptr);
    void enableCanvasesUpdates(bool enable);
    int canvasMinimumSize() const;
    QSize optimalSize() const;

    void resizeEvent(QResizeEvent *);

    Canvas *leftCanvas;
    Canvas *rightCanvas;
    QGridLayout *layout;
    LeftMenu *leftMenu;
    StatusBar *statusBar;
    TopMenu *topMenu;

    bool verticalCanvases;
};

#endif // MAINWINDOW_H
