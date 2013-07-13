#include <QPainter>
#include <QImage>
#include <QWheelEvent>
#include "canvas.h"
#include "canvasdelegate.h"


Canvas::Canvas(int width, int height, QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::WheelFocus);

    resize(width,height);
}

void Canvas::setDelegate(CanvasDelegate *delegate)
{
    this->delegate = delegate;
    return;
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter canvas_painter(this);
    canvas_painter.drawImage(0, 0, *(delegate->getImage()));
    return;
}

void Canvas::mousePressEvent(QMouseEvent *mouseevent)
{
    if(mouseevent->button()== Qt::LeftButton)
    {
        delegate->setMouse(mouseevent->x(),mouseevent->y());
    }
}

void Canvas::mouseMoveEvent(QMouseEvent * mouseevent)
{
    if(Qt::LeftButton == mouseevent->buttons())
    {
        delegate->moveMouse(mouseevent->x(), mouseevent->y());
    }
    repaint();
}

void Canvas::wheelEvent(QWheelEvent * wheelevent)
{
    double coeff = pow(1.2,wheelevent->delta()/120);
    delegate->setzoom(coeff,wheelevent->x(),wheelevent->y());
    repaint();
}
