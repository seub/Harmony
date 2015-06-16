#ifndef ACTIONHANDLER_H
#define ACTIONHANDLER_H

#include <QObject>

#include "tools.h"

class MathsContainer; class H2CanvasDelegate; class EquivariantHarmonicMapsFactory; class Window; class Canvas;

class ActionHandler : public QObject
{
    Q_OBJECT

    friend class Canvas;

public:
    ActionHandler();

    void setWindow(Window *window);
    void setContainer(MathsContainer *container);
    void setFactory();
    void processMessage(actionHandlerMessage message, int timeOut = 0);

private slots:
    void computeButtonClicked();
    void iterateButtonClicked();
    void outputResetButtonClicked();

private:
    void runDiscreteFlow();
    void iterateDiscreteFlow(int N);

    Window *window;
    MathsContainer *container;
    EquivariantHarmonicMapsFactory *factory;

    H2CanvasDelegate *leftDelegate, *rightDelegate;
};

#endif // ACTIONHANDLER_H
