#ifndef ACTIONHANDLER_H
#define ACTIONHANDLER_H

#include "tools.h"

class MathsContainer;
class H2CanvasDelegate;
class EquivariantHarmonicMapsFactory;
class Window;

class ActionHandler
{
    friend class Canvas;

public:
    ActionHandler();

    void setWindow(Window *window);
    void setContainer(MathsContainer *container);
    void setFactory();
    void processMessage(actionHandlerMessage message, int timeOut = 0);

private:
    Window *window;
    MathsContainer *container;
    EquivariantHarmonicMapsFactory *factory;
    H2CanvasDelegate *leftDelegate, *rightDelegate;
};

#endif // ACTIONHANDLER_H
