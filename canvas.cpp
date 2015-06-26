#include "canvas.h"

#include <QPainter>
#include <QImage>
#include <QWheelEvent>
#include <QDebug>

#include "h2canvasdelegate.h"
#include "h2canvasdelegateliftedgraph.h"
#include "actionhandler.h"
#include "window.h"
#include "fenchelnielsenuser.h"



Canvas::Canvas(DelegateType delegateType, Window *window, bool leftCanvas, bool rightCanvas, ActionHandler *handler) :
    container(window)
{
    setParent(window);
    initialize();

    delegate = nullptr;
    changeDelegate(delegateType, leftCanvas, rightCanvas, handler);
}

Canvas::Canvas(FenchelNielsenUser *fenchelNielsenUser) : container(fenchelNielsenUser)
{
    setParent(fenchelNielsenUser);
    initialize();

    delegate = new H2CanvasDelegateLiftedGraph(width(), height());
}

Canvas::~Canvas()
{
    delete delegate;
}

DelegateType Canvas::getDelegateType() const
{
    return delegate->getDelegateType();
}

void Canvas::initialize()
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setEnabled(true);
}

void Canvas::changeDelegate(DelegateType delegateType, bool leftCanvas, bool rightCanvas, ActionHandler *handler)
{
    delete delegate;

    switch(delegateType)
    {
    case DelegateType::GENERIC:
        delegate = new CanvasDelegate(width(), height(), leftCanvas, rightCanvas, handler);
        break;

    case DelegateType::H2DELEGATE:
        delegate = new H2CanvasDelegate(width(), height(), leftCanvas, rightCanvas, handler);
        break;

    case DelegateType::H2DELEGATE_GRAPH:
        delegate = new H2CanvasDelegateLiftedGraph(width(), height(), leftCanvas, rightCanvas, handler);
        break;

    case DelegateType::H3DELEGATE:
        break;

    default:
        throw(QString("ERROR in Canvas::changeDelegate: delegate type undefined"));
    }

    update();
}

void Canvas::updateRefresh(bool back, bool top)
{
    delegate->enableRedrawBuffer(back, top);
    update();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    //clock_t t0 = clock();

    delegate->redraw(delegate->enableRedrawBufferBack, delegate->enableRedrawBufferTop);
    delegate->enableRedrawBuffer(false, false);

    QPainter canvasPainter(this);
    canvasPainter.setClipRegion(event->region());
    canvasPainter.drawImage(0, 0, *(delegate->getImageBack()));
    canvasPainter.drawImage(0, 0, *(delegate->getImageTop()));

    if (delegate->getSendEndRepaint())
    {
        delegate->handler->processMessage(ActionHandlerMessage::END_CANVAS_REPAINT);
    }

    //qDebug() << "Time spent painting canvas: " << (clock() -t0)*1.0/CLOCKS_PER_SEC;
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
        container->canvasResized();
    }
    update();
}

void Canvas::moveEvent(QMoveEvent *)
{
    updateRefresh(true, true);
}

void Canvas::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    delegate->mouseMove(mouseEvent);
    update();
}

void Canvas::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    delegate->mouseRelease(mouseEvent);
    update();
}

void Canvas::enterEvent(QEvent *)
{
    setFocus();
    delegate->enter();
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
    update();
}

void Canvas::resetView()
{
    delegate->resetView();
    update();
}
