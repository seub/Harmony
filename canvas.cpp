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
    setMouseTracking(true);

    resize(width,height);
}

void Canvas::setDelegate(CanvasDelegate *delegate)
{
    this->delegate = delegate;
    return;
}

void Canvas::paintEvent(QPaintEvent *)
{
    //std::cout << "Entering Canvas::paintEvent" << std::endl;

    delegate->redrawBuffer();

    QPainter canvas_painter(this);
    canvas_painter.drawImage(0, 0, *(delegate->getImage()));

    //std::cout << "Leaving Canvas::paintEvent" << std::endl;
    return;
}

void Canvas::mousePressEvent(QMouseEvent *mouseEvent)
{
    delegate->mousePress(mouseEvent);
    return;
}

void Canvas::resizeEvent(QResizeEvent *resizeEvent)
{
    QSize newSize = resizeEvent->size();
    delegate->rescale(newSize.width(), newSize.height());
    //update();
    return;
}

void Canvas::mouseMoveEvent(QMouseEvent * mouseEvent)
{
    delegate->mouseMove(mouseEvent);
    update();
    return;
}

/*void Canvas::enterEvent(QEvent *)
{
    setFocus();
    return;
}*/

void Canvas::wheelEvent(QWheelEvent * wheelEvent)
{
    double coeff = pow(1.2,wheelEvent->delta()/120);
    delegate->zoom(coeff,wheelEvent->x(),wheelEvent->y());
    update();
}

void Canvas::keyPressEvent(QKeyEvent *keyEvent)
{
    delegate->keyPress(keyEvent);
    update();
    return;
}
