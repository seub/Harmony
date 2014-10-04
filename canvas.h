#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "tools.h"

class CanvasDelegate;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(int width = 700, int height = 700, QWidget *parent = 0);

    void setDelegate(CanvasDelegate *delegate);

signals:


private:
    CanvasDelegate *delegate;

    void paintEvent(QPaintEvent *event);
//public slots: (?)
    void resizeEvent(QResizeEvent *resizeEvent);
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void wheelEvent(QWheelEvent *wheelEvent);
    void keyPressEvent(QKeyEvent *keyEvent);
    //void enterEvent(QEvent *);
};

#endif // CANVAS_H
