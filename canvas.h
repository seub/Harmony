#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

class CanvasDelegate;
class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);

signals:
    
public slots:

private:
    void paintEvent(QPaintEvent *event);
    CanvasDelegate * delegate;
};

#endif // CANVAS_H
