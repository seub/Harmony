#include "canvas.h"

#include <QPainter>
#include <QImage>
#include <QWheelEvent>
#include <QDebug>
#include <QApplication>

#include "canvasdelegate.h"
#include "h2canvasdelegate.h"
#include "window.h"

Canvas::Canvas(CanvasDelegateType delegateType, Window *const window) :
    window(window)
{
    setParent(window);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setEnabled(true);

    //resize(width,height);

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

    case H2DELEGATEDOMAIN:
        delegate = new H2CanvasDelegateDomain(width(), height());
        break;

    case H2DELEGATETARGET:
        delegate = new H2CanvasDelegateTarget(width(), height());
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

    if (newSize.width()!=newSize.height())
    {
        window->resizeCanvases();
    }
    //update();
}

void Canvas::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    delegate->mouseMove(mouseEvent);
    update();
}

void Canvas::enterEvent(QEvent *)
{
    setFocus();
}

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

void Canvas::rescale()
{
    //delegate->rescale(width(), height());
    delegate->rescale(width(), width());
}
