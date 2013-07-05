#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

class H2Canvas;
class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);

signals:
    
public slots:

private:
    void paintEvent(QPaintEvent *event);
    H2Canvas * hypPlan;
};

#endif // CANVAS_H
