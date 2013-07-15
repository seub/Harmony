#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "tools.h"

class CanvasDelegate;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(int width = 600, int height = 600, QWidget *parent = 0);

    void setDelegate(CanvasDelegate * delegate);

signals:

public slots:
    //void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent * mouseEvent);
    void mouseMoveEvent(QMouseEvent * mouseEvent);
    void wheelEvent(QWheelEvent * wheelEvent);

private:
    CanvasDelegate * delegate;

    void paintEvent(QPaintEvent *event);
};

#endif // CANVAS_H
