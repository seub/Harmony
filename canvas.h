#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "tools.h"
#include "canvasdelegate.h"

class ActionHandler; class FenchelNielsenUser; class Window; class CanvasDelegateTests; class CanvasDelegateTests2;

class Canvas : public QWidget
{
    Q_OBJECT

    friend class Window;
    friend class FenchelNielsenUser;
    friend class ActionHandler;

public:
    Canvas(DelegateType delegateType, Window* window = nullptr, bool leftCanvas = false, bool rightCanvas = false, ActionHandler *handler = nullptr);
    explicit Canvas(FenchelNielsenUser *fenchelNielsenUser);
    explicit Canvas(CanvasDelegateTests *delegate);
    explicit Canvas(CanvasDelegateTests2 *delegate);
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
    void resizeEvent(QResizeEvent *) override;
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

    int imageFirstCornerX() const;
    int imageFirstCornerY() const;
    int imageMaxSize() const;

    bool mouseEventOverImage(QMouseEvent *mouseEvent, int &xOut, int &yOut) const;

    CanvasDelegate *delegate;
};

#endif // CANVAS_H
