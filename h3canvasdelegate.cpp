#include "h3canvasdelegate.h"

H3canvasDelegate::H3canvasDelegate(int framesPerSecond, QWidget *parent, char *name)
    : QGLWidget(parent)
{
    isClicked = false; //To change this Check if the mouse button is clicked. Perhaps use Qt function instead of this flag.

    //Set Title
    setWindowTitle(QString::fromUtf8(name));

    //Set refresh frequence
    if(framesPerSecond == 0)
        t_Timer = NULL;
    else
    {
        int seconde = 1000; // 1 seconde = 1000 ms
        timerInterval = seconde / framesPerSecond;
        t_Timer = new QTimer(this);
        connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
        t_Timer->start( timerInterval );
    }
    mousePosition[0] = 3;
    mousePosition[1] = 2;
    mousePosition[2] = -4;
}

H3canvasDelegate::~H3canvasDelegate()
{
}


void H3canvasDelegate::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Up:
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glRotated(1, 0,1,0);
        glPushMatrix();
        break;
    case Qt::Key_Down:
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glRotated(-1, 0,1,0);
        glPushMatrix();
        break;
    case Qt::Key_Left:
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glRotated(-1, 0,0,1);
        glPushMatrix();
        break;
    case Qt::Key_Right:
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glRotated(1, 0,0,1);
        glPushMatrix();
        break;
    }
}

void H3canvasDelegate::wheelEvent(QWheelEvent * wheelevent)
{
    if(wheelevent->delta() >0)
    {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glScalef(1.05,1.05,1.05);
        glPushMatrix();
    }
    else
    {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glScalef(0.95,0.95,0.95);
        glPushMatrix();
    }
}


void H3canvasDelegate::mouseGLCoordinates(QMouseEvent *mouseevent, GLdouble *pos)
{
    GLint viewport[4];
    //GLdouble modelview[16];
    GLdouble projection[16];
    //glGetDoublev( GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluUnProject(mouseevent->x(),viewport[3]- mouseevent->y(),0.1f,modelviewsave,projection,viewport,pos,pos +1,pos + 2);

}

void H3canvasDelegate::mousePressEvent(QMouseEvent * mouseevent)
{
    isClicked = true;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glGetDoublev( GL_MODELVIEW_MATRIX, modelviewsave);
    //get mouse position in OpenGL coordinates
    mouseGLCoordinates(mouseevent,mousePosition);
    mouseGLCoordinates(mouseevent,currentPosition);
    angle = 0;
}

void H3canvasDelegate::mouseReleaseEvent(QMouseEvent * /*mouseevent*/)
{
    isClicked = false;
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glLoadMatrixd(modelviewsave);
    glRotated(angle, normalvect[0],normalvect[1],normalvect[2]);
    glPushMatrix();

}

void H3canvasDelegate::mouseMoveEvent(QMouseEvent * mouseevent)
{
    if(isClicked)
    {
        mouseGLCoordinates(mouseevent, currentPosition);
        normalvect[0] = mousePosition[1]*currentPosition[2] - mousePosition[2]*currentPosition[1];
        normalvect[1] = mousePosition[2]*currentPosition[0] - mousePosition[0]*currentPosition[2];
        normalvect[2] = mousePosition[0]*currentPosition[1] - mousePosition[1]*currentPosition[0];
        std::cout << "mousePos" << mousePosition[0] << " " << mousePosition[1] << " " << mousePosition[2] << std::endl;
        std::cout << "currentPosition" << currentPosition[0] << " " << currentPosition[1] << " " << currentPosition[2] << std::endl;
        std::cout << "normalvect" << normalvect[0] << " " << normalvect[1] << " " << normalvect[2] << std::endl;
        GLdouble posNorm = sqrt(mousePosition[0]*mousePosition[0]
                               + mousePosition[1]*mousePosition[1]
                               + mousePosition[2]*mousePosition[2]);
        GLdouble curNorm = sqrt(currentPosition[0]*currentPosition[0]
                               + currentPosition[1]*currentPosition[1]
                               + currentPosition[2]*currentPosition[2]);
        GLdouble norNorm = sqrt(normalvect[0]*normalvect[0]
                               + normalvect[1]*normalvect[1]
                               + normalvect[2]*normalvect[2]);

        angle = 180*asin(norNorm/(curNorm*posNorm))/M_PI*100;
        std::cout << angle << " " << posNorm << " " << curNorm << " " << norNorm << std::endl;
    }
}

void H3canvasDelegate::timeOutSlot()//Called at each refresh
{
    updateGL(); //Gl refreash function.
    glFlush(); //
}

void H3canvasDelegate::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


void H3canvasDelegate::resizeGL(int width, int height)
{
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3.0f,3.0f,6.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f);
    glPushMatrix();
}

void H3canvasDelegate::printAxis(double length)
{
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2d(0,0);glVertex2d(length,0);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2d(0,0);glVertex2d(0,length);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2d(0,0);glVertex3d(0,0,length);
    glEnd();
}

void H3canvasDelegate::printCube()
{
    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3d(1.0f, -1.0f, -1.0f);
    glEnd();
}

void H3canvasDelegate::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(isClicked)
    {
        glPopMatrix();
        glLoadMatrixd(modelviewsave);
        //glPushMatrix();
        //glPopMatrix();
        /*glBegin(GL_LINES);
            glColor3f(1.0f, 0.0f, 1.0f);
            double x = mousePosition[0],y = mousePosition[1],z = mousePosition[2];
            glVertex2d(0,0);glVertex3d(x,y,z);
            glColor3f(1.0f, 1.0f, 0.0f);
            glVertex2d(0,0);glVertex3d(currentPosition[0],currentPosition[1],currentPosition[2]);
            glColor3f(1.0f, 0.0f, 1.0f);
            glVertex2d(0,0);glVertex3d(normalvect[0],normalvect[1],normalvect[2]);
        glEnd();*/
        glRotated(angle, normalvect[0],normalvect[1],normalvect[2]);
    }
    else
    {
        glPopMatrix();
    }
    //glLoadIdentity();
    //gluLookAt(rho*cos(theta)*sin(phi)+xcenter,rho*sin(theta)*sin(phi)+ycenter,rho*cos(phi)+zcenter,xcenter,ycenter,zcenter,0,0,sin(phi)>0?1:-1);
    printCube();
    printAxis(2.);

    glPushMatrix();
}
