#include "canvas.h"

#include <QPainter>
#include <QImage>
#include <QWheelEvent>
#include <QDebug>
#include <QApplication>

#include "h2canvasdelegate.h"
#include "h2canvasdelegateliftedgraph.h"
#include "actionhandler.h"
#include "window.h"
#include "fenchelnielsenuser.h"
#include "canvasdelegatetests.h"



Canvas::Canvas(DelegateType delegateType, Window *window, bool leftCanvas, bool rightCanvas, ActionHandler *handler)
{
    setParent(window);
    initialize();
    
    delegate = nullptr;
    changeDelegate(delegateType, leftCanvas, rightCanvas, handler);
}

Canvas::Canvas(FenchelNielsenUser *fenchelNielsenUser)
{
    setParent(fenchelNielsenUser);
    initialize();
    
    int size = std::min(width(), height());
    
    delegate = new H2CanvasDelegateLiftedGraph(size, size);
}

Canvas::Canvas(CanvasDelegateTests *delegate)
{
    resize(delegate->sizeX, delegate->sizeY);
    this->delegate = delegate;
    initialize();
}

Canvas::Canvas(CanvasDelegateTests2 *delegate)
{
    resize(delegate->sizeX, delegate->sizeY);
    this->delegate = delegate;
    initialize();
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
    //setBackgroundRole(QPalette::Base);
    //setAutoFillBackground(true);
    setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);
    setEnabled(true);
}

void Canvas::changeDelegate(DelegateType delegateType, bool leftCanvas, bool rightCanvas, ActionHandler *handler)
{
    delete delegate;
    
    int size = std::min(width(), height());
    
    switch(delegateType)
    {
    case DelegateType::GENERIC:
        delegate = new CanvasDelegate(size, size, leftCanvas, rightCanvas, handler);
        break;
        
    case DelegateType::H2DELEGATE:
        delegate = new H2CanvasDelegate(size, size, leftCanvas, rightCanvas, handler);
        break;
        
    case DelegateType::H2DELEGATE_GRAPH:
        delegate = new H2CanvasDelegateLiftedGraph(size, size, leftCanvas, rightCanvas, handler);
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

int Canvas::imageFirstCornerX() const
{
    return width() > height() ? (width()-height())/2 : 0;
}

int Canvas::imageFirstCornerY() const
{
    return height() > width() ? (height()-width())/2 : 0;
}

int Canvas::imageMaxSize() const
{
    return std::min(width(), height());
}

bool Canvas::mouseEventOverImage(QMouseEvent *mouseEvent, int &xOut, int &yOut) const
{
    int x = mouseEvent->x();
    int y = mouseEvent->y();

    if ((x < imageFirstCornerX()) || (x > imageFirstCornerX() + imageMaxSize()) || (y < imageFirstCornerY()) || (y > imageFirstCornerY() + imageMaxSize()))
    {
        return false;
    }
    else
    {
        xOut = x - imageFirstCornerX();
        yOut = y - imageFirstCornerY();
        return true;
    }
}

void Canvas::paintEvent(QPaintEvent *event)
{
    //clock_t t0 = clock();
    
    delegate->redraw(delegate->enableRedrawBufferBack, delegate->enableRedrawBufferTop);
    delegate->enableRedrawBuffer(false, false);
    
    
    
    //Tests
    int imageSize = delegate->getImageBack()->width();
    if (imageSize != imageMaxSize())
    {
        qDebug() << "Error in Canvas::paintEvent: image size doesn't fit canvas size";
    }
    
    QPainter canvasPainter(this);
    canvasPainter.setClipRegion(event->region());
    canvasPainter.drawImage(imageFirstCornerX(), imageFirstCornerY(), *(delegate->getImageBack()));
    canvasPainter.drawImage(imageFirstCornerX(), imageFirstCornerY(), *(delegate->getImageTop()));
    
    if (delegate->getSendEndRepaint())
    {
        delegate->handler->processMessage(ActionHandlerMessage::END_CANVAS_REPAINT);
    }
    
    //qDebug() << "Time spent painting canvas: " << (clock() -t0)*1.0/CLOCKS_PER_SEC;
}

void Canvas::mousePressEvent(QMouseEvent *mouseEvent)
{
    int x, y;
    if (mouseEventOverImage(mouseEvent, x, y))
    {
        delegate->mousePress(x, y, mouseEvent->button(), mouseEvent->buttons());
    }
}

void Canvas::resizeEvent(QResizeEvent *)
{
    rescale();
    updateRefresh(true, true);
}

void Canvas::moveEvent(QMoveEvent *)
{
    updateRefresh(true, true);
}

void Canvas::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    int x, y;
    if (mouseEventOverImage(mouseEvent, x, y))
    {
        delegate->mouseMove(x, y, mouseEvent->button(), mouseEvent->buttons());
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    int x, y;
    if (mouseEventOverImage(mouseEvent, x, y))
    {
        delegate->mouseRelease(x, y, mouseEvent->button(), mouseEvent->buttons());
        update();
    }
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
    int size = std::min(width(), height());
    delegate->rescale(size, size);
    update();
}

void Canvas::resetView()
{
    delegate->resetView();
    update();
}
