#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include "tools.h"
#include "equivariantharmonicmapsfactory.h"
#include "canvascontainer.h"

class QGridLayout; class QStatusBar; class QLabel;

class Canvas; class InputMenu; class OutputMenu; class TopMenu; class DisplayMenu;
class ActionHandler;

class Window : public QWidget, public CanvasContainer
{
    Q_OBJECT

    friend class ActionHandler;
    friend class MainApplication;
    friend class Canvas;

public:
    Window(ActionHandler* handler = 0);

    void resizeEvent(QResizeEvent *event);
signals:

public slots:

private:
    void createWindow(ActionHandler *handler = 0);
    void enableCanvasesUpdates(bool b);
    void canvasResized();
    int menuWidth() const;
    void optimalSize(unsigned int &outputWidth, unsigned int &outputHeight) const;

    Canvas *leftCanvas;
    Canvas *rightCanvas;
    QGridLayout* layout;
    InputMenu* inputMenu;
    DisplayMenu *displayMenu;
    OutputMenu* outputMenu;
    QStatusBar* statusBar;
    QLabel* statusBarLabel;
    TopMenu* topMenu;
};

#endif // WINDOW_H
