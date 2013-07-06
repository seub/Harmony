#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "types.h"

class CanvasDelegate;
class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(int width = 600, int height = 600, QWidget *parent = 0);

    void setDelegate(CanvasDelegate * delegate);

signals:
    
public slots:

private:
    CanvasDelegate * delegate;


    void paintEvent(QPaintEvent *event);
};

#endif // CANVAS_H
