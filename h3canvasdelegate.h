#ifndef H3CANVASDELEGATE_H
#define H3CANVASDELEGATE_H

#include <QtOpenGL>
#include <QGLWidget>
#include <GL/glu.h>
#include "tools.h"

class H3canvasDelegate : public QGLWidget
{
    Q_OBJECT
public:
    explicit H3canvasDelegate(int framesPerSecond = 0, QWidget *parent = 0, char *name = 0);
    ~H3canvasDelegate();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void printAxis(double length);
    void printCube();
    void keyPressEvent( QKeyEvent *keyEvent );
    void mouseGLCoordinates(QMouseEvent * mouseevent, GLdouble *pos, GLdouble *modelView = 0);
public slots:
    void timeOutSlot();
    void mousePressEvent(QMouseEvent * mouseevent);
    void mouseReleaseEvent(QMouseEvent * mouseevent);
    void mouseMoveEvent(QMouseEvent * mouseevent);
    void wheelEvent(QWheelEvent * wheelevent);
private:
    QTimer *t_Timer;
    int timerInterval;

    bool isClicked;
    GLdouble angle;
    GLdouble mousePosition[3];
    GLdouble normalvect[3];
    GLdouble currentPosition[3];
    GLdouble modelviewsave[16];
    void drawSphere(GLdouble xcenter, GLdouble ycenter, GLdouble zcenter, GLdouble radius, GLenum  style = GLU_SILHOUETTE);
    void drawCircleArc(GLdouble xcenter, GLdouble ycenter, GLdouble zcenter,
                       GLdouble xnormal, GLdouble ynormal, GLdouble znormal,
                       GLdouble xfirst, GLdouble yfirst, GLdouble zfirst,//relative to the center
                       GLdouble angle);
};

#endif // H3CANVASDELEGATE_H
