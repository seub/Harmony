#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "tools.h"

class CanvasDelegate;
class Window;
class ActionHandler;
class FenchelNielsenUser;
class CanvasContainer;

class Canvas : public QWidget
{
    Q_OBJECT

    friend class Window;
    friend class FenchelNielsenUser;

public:
    explicit Canvas(CanvasDelegateType delegateType, Window *window = 0, ActionHandler* handler = 0);
    explicit Canvas(FenchelNielsenUser *FNuser);
    ~Canvas();

    void changeDelegate(CanvasDelegateType delegateType, ActionHandler *handler = 0);
    CanvasDelegateType getDelegateType() const;

signals:


private:
public:
    CanvasDelegate *delegate;

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *resizeEvent);
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void wheelEvent(QWheelEvent *wheelEvent);
    void keyPressEvent(QKeyEvent *keyEvent);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

    void resetView();

private:
    Canvas(); // Dummy constructor
    Canvas(const Canvas &other); // Copy constructor
    Canvas & operator=(Canvas other); // Copy-assignment operator

    void initialize();
    void rescale();

    CanvasContainer* container;
};

#endif // CANVAS_H
