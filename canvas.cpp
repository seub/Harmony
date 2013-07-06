#include <QPainter>
#include <QImage>

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

