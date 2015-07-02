#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include "tools.h"

class QGridLayout;

class Canvas; class LeftMenu; class TopMenu;
class ActionHandler; class StatusBar;

class Window : public QWidget
{
    Q_OBJECT

    friend class ActionHandler;
    friend class MainApplication;
    friend class Canvas;

public:
    Window(ActionHandler *handler = nullptr);
    Window(const Window &) = delete;
    Window & operator =(Window) = delete;

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

#endif // WINDOW_H
