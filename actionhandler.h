#ifndef ACTIONHANDLER_H
#define ACTIONHANDLER_H

#include "tools.h"

class MathsContainer;
class H2CanvasDelegate;
class EquivariantHarmonicMapsFactory;
class Window;

class ActionHandler
{
public:
    ActionHandler();

    void setWindow(Window *window);
    void setContainer(MathsContainer *container);
    void setFactory();

private:
    Window *window;
    MathsContainer *container;
    EquivariantHarmonicMapsFactory *factory;
    H2CanvasDelegate *leftDelegate, *rightDelegate;
};

#endif // ACTIONHANDLER_H
