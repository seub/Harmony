#ifndef CANVASCONTAINER_H
#define CANVASCONTAINER_H

class CanvasContainer
{
public:
    CanvasContainer();
    virtual void canvasResized() = 0;
};

#endif // CANVASCONTAINER_H
