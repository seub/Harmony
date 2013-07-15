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

void Canvas::mousePressEvent(QMouseEvent *mouseEvent)
{
    if(mouseEvent->button()== Qt::LeftButton)
    {
        delegate->setMouse(mouseEvent->x(),mouseEvent->y());
    }
}

void Canvas::mouseMoveEvent(QMouseEvent * mouseEvent)
{
    if(Qt::LeftButton == mouseEvent->buttons())
    {
        delegate->moveMouse(mouseEvent->x(), mouseEvent->y());
    }
    repaint();
}

void Canvas::wheelEvent(QWheelEvent * wheelEvent)
{
    double coeff = pow(1.2,wheelEvent->delta()/120);
    delegate->setzoom(coeff,wheelEvent->x(),wheelEvent->y());
    repaint();
}
