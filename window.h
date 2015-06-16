#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include "tools.h"
#include "equivariantharmonicmapsfactory.h"

class QGridLayout; class QStatusBar; class QLabel;

class Canvas; class InputMenu; class OutputMenu; class TopMenu;
class ActionHandler;

class Window : public QWidget
{
    Q_OBJECT
public:
    Window(ActionHandler* handler = 0);

    void resizeEvent(QResizeEvent *event);
signals:

public slots:

private:
public:
    void createWindow(ActionHandler *handler = 0);
    void enableCanvasesUpdates(bool b);
    void resizeCanvases();
    int menuWidth() const;
    void optimalSize(unsigned int &outputWidth, unsigned int &outputHeight) const;

    Canvas *leftCanvas;
    Canvas *rightCanvas;
    QGridLayout* layout;
    InputMenu* inputMenu;
    OutputMenu* outputMenu;
    QStatusBar* statusBar;
    QLabel* statusBarLabel;
    TopMenu* topMenu;
};

#endif // WINDOW_H
