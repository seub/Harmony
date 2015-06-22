#include "canvas.h"

#include <QPainter>
#include <QImage>
#include <QWheelEvent>
#include <QDebug>
#include <QApplication>

#include "h2canvasdelegate.h"
#include "window.h"
#include "actionhandler.h"
#include "fenchelnielsenuser.h"
#include "canvascontainer.h"

Canvas::Canvas(DelegateType delegateType, Window *window, ActionHandler *handler) :
    container(window)
{
    setParent(window);
    initialize();

    delegate = nullptr;
    changeDelegate(delegateType, handler);
}

Canvas::Canvas(FenchelNielsenUser *user) : container(user)
{
    setParent(user);
    initialize();

    delegate = new H2CanvasDelegate(width(), height());
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


Canvas::~Canvas()
{
    delete delegate;
}

void Canvas::changeDelegate(DelegateType delegateType, ActionHandler *handler)
{

    delete delegate;

    switch(delegateType)
    {
    case DelegateType::GENERIC:
        delegate = new CanvasDelegate(width(), height(), handler);
        break;

    case DelegateType::H2DELEGATE:
        delegate = new H2CanvasDelegate(width(), height(), handler);
        break;

    case DelegateType::H2DELEGATEDOMAIN:
        delegate = new H2CanvasDelegateDomain(width(), height(), handler);
        break;

    case DelegateType::H2DELEGATETARGET:
        delegate = new H2CanvasDelegateTarget(width(), height(), handler);
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


    delegate->redrawBuffer(delegate->enableRedrawBufferBack, delegate->enableRedrawBufferTop);
    delegate->enableRedrawBuffer(false, false);

    QPainter canvasPainter(this);
    canvasPainter.setClipRegion(event->region());
    canvasPainter.drawImage(0, 0, *(delegate->getImageBack()));
    canvasPainter.drawImage(0, 0, *(delegate->getImageTop()));

    if (getDelegateType() == DelegateType::H2DELEGATETARGET)
    {
        delegate->handler->processMessage(ActionHandlerMessage::END_TARGET_CANVAS_REPAINT);
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
