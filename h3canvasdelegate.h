#ifndef H3CANVASDELEGATE_H
#define H3CANVASDELEGATE_H

#include <QtOpenGL>
#include <QGLWidget>
#include <GL/glu.h>
#include "tools.h"


class GLVector
{
public :
    GLVector(GLdouble x = 0.0d, GLdouble y = 0.0d, GLdouble z = 0.0d);
    GLdouble x;
    GLdouble y;
    GLdouble z;
    GLdouble norm() const { return sqrt(x*x+y*y+z*z);}
    GLdouble scalarProduct(const GLVector &v) const;
    GLVector vectorProduct(const GLVector &v) const;
    GLVector add(const GLVector &v) const;
    GLVector opposite() const;
    GLVector scalarMult(const GLdouble lambda) const;
};

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
    void mouseGLCoordinates(const QMouseEvent * mouseevent, GLVector &pos, GLdouble *modelView = 0) const;
public slots:
    void timeOutSlot();
    void mousePressEvent(QMouseEvent * mouseevent);
    void mouseReleaseEvent(QMouseEvent * mouseevent);
    void mouseMoveEvent(QMouseEvent * mouseevent);
    void wheelEvent(QWheelEvent * wheelevent);
private:
    QTimer *t_Timer;
    int timerInterval;
    void glVertex3(const GLVector v);
    bool isClicked;
    GLdouble angle;
    GLVector mousePosition;
    GLVector normalvect;
    GLVector currentPosition;
    GLdouble modelviewsave[16];
    void drawSphere(const GLVector center,GLdouble radius,GLenum  style = GLU_SILHOUETTE);
    void drawCircleArc(GLVector center,GLVector normal, GLVector first /*relative to the center*/, GLdouble angle);
};




#endif // H3CANVASDELEGATE_H
