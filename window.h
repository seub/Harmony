#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include "tools.h"
#include "equivariantharmonicmapsfactory.h"

class QGridLayout; class QStatusBar; class QLabel;

class Canvas; class InputMenu; class OutputMenu; class TopMenu;

class Window : public QWidget
{
    Q_OBJECT
public:
    Window();

    void resizeEvent(QResizeEvent *event);
    void setFactory(EquivariantHarmonicMapsFactory *Factory);

signals:

public slots:

//private:
public:
    void createWindow();
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

    EquivariantHarmonicMapsFactory *Factory;
};

#endif // WINDOW_H
