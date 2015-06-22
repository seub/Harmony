#ifndef CANVASCONTAINER_H
#define CANVASCONTAINER_H

class CanvasContainer
{
    friend class Window;
    friend class FenchelNielsenUser;
    friend class Canvas;

public:
    CanvasContainer(const CanvasContainer &) = delete;
    CanvasContainer & operator=(CanvasContainer &) = delete;

private:
    CanvasContainer();
    virtual void canvasResized() = 0;
};

#endif // CANVASCONTAINER_H
