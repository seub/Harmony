#include "canvas.h"

#include <QPainter>
#include <QImage>
#include <QWheelEvent>
#include <QDebug>
#include <QApplication>

#include "canvasdelegate.h"
#include "h2canvasdelegate.h"
#include "window.h"
#include "actionhandler.h"

Canvas::Canvas(CanvasDelegateType delegateType, Window *const window, ActionHandler *handler) :
    window(window)
{
    setParent(window);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setEnabled(true);

    delegate = 0;
    changeDelegate(delegateType, handler);
}

Canvas::~Canvas()
{
    delete delegate;
}

void Canvas::changeDelegate(CanvasDelegateType delegateType, ActionHandler *handler)
{

    delete delegate;

    switch(delegateType)
    {
    case H2DELEGATE:
        delegate = new H2CanvasDelegate(width(), height(), handler);
        break;

    case H2DELEGATEDOMAIN:
        delegate = new H2CanvasDelegateDomain(width(), height(), handler);
        break;

    case H2DELEGATETARGET:
        delegate = new H2CanvasDelegateTarget(width(), height(), handler);
        break;

    case H3DELEGATE:
        break;

    default:
        throw(QString("ERROR in Canvas::changeDelegate: delegate type undefined"));
    }
}


void Canvas::paintEvent(QPaintEvent *event)
{
    //std::cout << "Entering Canvas::paintEvent at time " << clock()*1.0/CLOCKS_PER_SEC << std::endl;

    delegate->redrawBuffer(delegate->enableRedrawBufferBack, delegate->enableRedrawBufferTop);
    delegate->enableRedrawBuffer(false, false);

    QPainter canvasPainter(this);
    canvasPainter.setClipRegion(event->region());
    canvasPainter.drawImage(0, 0, *(delegate->getImageBack()));
    canvasPainter.drawImage(0, 0, *(delegate->getImageTop()));

    delegate->handler->processMessage(END_CANVAS_REPAINT, delegate->delegateType);

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

void Canvas::leaveEvent(QEvent *)
{
    delegate->leave();
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
    //update();
}
