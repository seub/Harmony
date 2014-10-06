#include <QPainter>
#include <QImage>
#include <QWheelEvent>
#include "canvas.h"
#include "canvasdelegate.h"
#include "h2canvasdelegate.h"

Canvas::Canvas(CanvasDelegateType delegateType, int width, int height, QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);

    resize(width,height);

    delegate = 0;
    changeDelegate(delegateType);
}

Canvas::~Canvas()
{
    delete delegate;
}

void Canvas::changeDelegate(CanvasDelegateType delegateType)
{
    delete delegate;

    switch(delegateType)
    {
    case H2DELEGATE:
        delegate = new H2CanvasDelegate(width(), height());
        break;

    case H3DELEGATE:
        break;

    default:
        std::cout << "ERROR in Canvas::changeDelegate: delegate type undefined" << std::endl;
    }
}


void Canvas::paintEvent(QPaintEvent *)
{
    //std::cout << "Entering Canvas::paintEvent" << std::endl;

    delegate->redrawBuffer();

    QPainter canvas_painter(this);
    canvas_painter.drawImage(0, 0, *(delegate->getImage()));

    //std::cout << "Leaving Canvas::paintEvent" << std::endl;
}

void Canvas::mousePressEvent(QMouseEvent *mouseEvent)
{
    delegate->mousePress(mouseEvent);
}

void Canvas::resizeEvent(QResizeEvent *resizeEvent)
{
    QSize newSize = resizeEvent->size();
    delegate->rescale(newSize.width(), newSize.height());
    //update();
}

void Canvas::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    delegate->mouseMove(mouseEvent);
    update();
}

/*void Canvas::enterEvent(QEvent *)
{
    setFocus();
}*/

void Canvas::wheelEvent(QWheelEvent *wheelEvent)
{
    double coeff = pow(1.2,wheelEvent->delta()/120);
    delegate->zoom(coeff,wheelEvent->x(),wheelEvent->y());
    update();
}

void Canvas::keyPressEvent(QKeyEvent *keyEvent)
{
    delegate->keyPress(keyEvent);
    update();
}
