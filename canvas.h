#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "tools.h"
#include "canvasdelegate.h"

class ActionHandler; class FenchelNielsenUser; class CanvasContainer; class Window;

class Canvas : public QWidget
{
    Q_OBJECT

    friend class Window;
    friend class FenchelNielsenUser;
    friend class ActionHandler;

public:
    Canvas(DelegateType delegateType, Window* window = nullptr, bool leftCanvas = false, bool rightCanvas = false, ActionHandler *handler = nullptr);
    explicit Canvas(FenchelNielsenUser *fenchelNielsenUser);
    Canvas() = delete;
    Canvas(const Canvas &) = delete;
    Canvas & operator=(Canvas) = delete;
    ~Canvas();

    DelegateType getDelegateType() const;
    void resetView();

private:
    void changeDelegate(DelegateType delegateType, bool leftCanvas, bool rightCanvas, ActionHandler *handler = nullptr);

    void updateRefresh(bool back, bool top);
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *resizeEvent) override;
    void moveEvent(QMoveEvent *) override;
    void mousePressEvent(QMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
    void wheelEvent(QWheelEvent *wheelEvent) override;
    void keyPressEvent(QKeyEvent *keyEvent) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    void initialize();
    void rescale();

    CanvasContainer *const container;
    CanvasDelegate *delegate;
};

#endif // CANVAS_H
