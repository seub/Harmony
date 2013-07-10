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
        glRotated(2, 0,1,0);
        glPushMatrix();
        break;
    case Qt::Key_Down:
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glRotated(-2, 0,1,0);
        glPushMatrix();
        break;
    case Qt::Key_Left:
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glRotated(-2, 0,0,1);
        glPushMatrix();
        break;
    case Qt::Key_Right:
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glRotated(2, 0,0,1);
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


void H3canvasDelegate::mouseGLCoordinates(QMouseEvent *mouseevent, GLdouble *pos, GLdouble *modelView)
{
    bool createModelView = false;
    GLint viewport[4];
    /*GLdouble Identity[16];
    int i;
    for(i = 0; i< 16;i++)
    {
        Identity[i]=0;
    }
    Identity[0]=1;
    Identity[5]=1;
    Identity[10]=1;
    Identity[15]=1;*/
    GLdouble projection[16];
    if(modelView == 0)
    {
        createModelView = true;
        modelView = new GLdouble[16];
        glGetDoublev( GL_MODELVIEW_MATRIX, modelView);
    }
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluProject(0.0d,0.0d,0.0d,modelView,projection,viewport,pos,pos +1,pos + 2);
    gluUnProject(mouseevent->x(),viewport[3]- mouseevent->y(),pos[2],modelView,projection,viewport,pos,pos +1,pos + 2);
    if(createModelView)
    {
        delete modelView;
    }
}

void H3canvasDelegate::mousePressEvent(QMouseEvent * mouseevent)
{
    isClicked = true;
    glGetDoublev( GL_MODELVIEW_MATRIX, modelviewsave);
    //get mouse position in OpenGL coordinates
    mouseGLCoordinates(mouseevent,mousePosition, modelviewsave);
    mouseGLCoordinates(mouseevent,currentPosition, modelviewsave);
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
        mouseGLCoordinates(mouseevent, currentPosition, modelviewsave);
        normalvect[0] = mousePosition[1]*currentPosition[2] - mousePosition[2]*currentPosition[1];
        normalvect[1] = mousePosition[2]*currentPosition[0] - mousePosition[0]*currentPosition[2];
        normalvect[2] = mousePosition[0]*currentPosition[1] - mousePosition[1]*currentPosition[0];
        GLdouble posNorm = sqrt(mousePosition[0]*mousePosition[0]
                               + mousePosition[1]*mousePosition[1]
                               + mousePosition[2]*mousePosition[2]);
        GLdouble curNorm = sqrt(currentPosition[0]*currentPosition[0]
                               + currentPosition[1]*currentPosition[1]
                               + currentPosition[2]*currentPosition[2]);
        GLdouble norNorm = sqrt(normalvect[0]*normalvect[0]
                               + normalvect[1]*normalvect[1]
                               + normalvect[2]*normalvect[2]);
        double scal = (normalvect[0]*currentPosition[0]
                + normalvect[1]*currentPosition[1]
                + normalvect[2]*currentPosition[2]);

        if(posNorm > curNorm)
        {
            double lambda[2];
            lambda[0] =(sqrt((posNorm*posNorm - curNorm*curNorm)))/norNorm;
            lambda[1] = -lambda[0];

                GLint viewport[4];
                GLdouble projection[16];
                glGetDoublev(GL_PROJECTION_MATRIX, projection);
                glGetIntegerv(GL_VIEWPORT, viewport);
                double x,y,z[2];
                gluProject(currentPosition[0]+lambda[0]*normalvect[0],currentPosition[1]+lambda[0]*normalvect[1],
                        currentPosition[2]+lambda[0]*normalvect[2],modelviewsave,projection,viewport,&x,&y,z);
                gluProject(currentPosition[0]+lambda[1]*normalvect[0],currentPosition[1]+lambda[1]*normalvect[1],
                        currentPosition[2]+lambda[1]*normalvect[2],modelviewsave,projection,viewport,&x,&y,z+1);
                std::cout << lambda[0] << " " << lambda[1] << std::endl;
                int i = z[0]<z[1]?0:1;
                currentPosition[0]+=lambda[i]*normalvect[0];
                currentPosition[1]+=lambda[i]*normalvect[1];
                currentPosition[2]+=lambda[i]*normalvect[2];
                normalvect[0] = mousePosition[1]*currentPosition[2] - mousePosition[2]*currentPosition[1];
                normalvect[1] = mousePosition[2]*currentPosition[0] - mousePosition[0]*currentPosition[2];
                normalvect[2] = mousePosition[0]*currentPosition[1] - mousePosition[1]*currentPosition[0];

                curNorm = sqrt(currentPosition[0]*currentPosition[0]
                        + currentPosition[1]*currentPosition[1]
                        + currentPosition[2]*currentPosition[2]);
        }
        else
        {
            mousePosition[0]*=curNorm/posNorm;
            mousePosition[1]*=curNorm/posNorm;
            mousePosition[2]*=curNorm/posNorm;
        }
        //angle = 180*asin(norNorm/(curNorm*posNorm))/M_PI*100;
        angle = 180*acos((mousePosition[0]*currentPosition[0]
                + mousePosition[1]*currentPosition[1]
                + mousePosition[2]*currentPosition[2])/(posNorm*curNorm))/M_PI;
        if(angle!=angle) angle =0;
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
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 1.1f, 100.0f);
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

void H3canvasDelegate::drawSphere(GLdouble xcenter, GLdouble ycenter, GLdouble zcenter, GLdouble radius ,GLenum style)
{
    GLUquadricObj *quadric;
    quadric = gluNewQuadric();
    glTranslated(xcenter,ycenter,zcenter);
    gluQuadricDrawStyle(quadric, style );

    gluSphere( quadric , radius , 10, 10 );
    glTranslated(-xcenter,-ycenter,-zcenter);

    gluDeleteQuadric(quadric);
    glEndList();
}

void H3canvasDelegate::drawCircleArc(GLdouble xcenter, GLdouble ycenter, GLdouble zcenter,
                                     GLdouble xnormal, GLdouble ynormal, GLdouble znormal,
                                     GLdouble xfirst, GLdouble yfirst, GLdouble zfirst, //relative to the center
                                     GLdouble angle)
{
    angle = angle<2*M_PI?angle:2*M_PI;
    GLdouble norm = sqrt(xnormal*xnormal+ynormal*ynormal+znormal*znormal);
    GLdouble xprod = (ynormal*zfirst - znormal*yfirst)/norm;
    GLdouble yprod = (znormal*xfirst - xnormal*zfirst)/norm;
    GLdouble zprod = (xnormal*yfirst - ynormal*xfirst)/norm;
    double i;
    glBegin(GL_LINE_STRIP);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3d(xcenter+xfirst, ycenter+yfirst, zcenter+zfirst);
        for(i =0; i < angle; i+=0.01)
        {
            glVertex3d(cos(i)*xfirst + sin(i)*xprod + xcenter,
                       cos(i)*yfirst + sin(i)*yprod + ycenter,
                       cos(i)*zfirst + sin(i)*zprod + zcenter);
        }
        glVertex3d(cos(i)*xfirst + sin(i)*xprod + xcenter,
                               cos(i)*yfirst + sin(i)*yprod + ycenter,
                               cos(i)*zfirst + sin(i)*zprod + zcenter);
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
    glPopMatrix();
    if(isClicked)
    {      
        glLoadMatrixd(modelviewsave);
        /*glBegin(GL_LINES);
            glColor3f(1.0f, 0.0f, 1.0f);
            float x = mousePosition[0],y = mousePosition[1],z = mousePosition[2];
            glVertex2f(0.0f,0.0f);glVertex3f(x,y,z);
            glColor3f(1.0f, 1.0f, 0.0f);
            glVertex2d(0,0);glVertex3d(currentPosition[0],currentPosition[1],currentPosition[2]);
            glColor3f(1.0f, 0.0f, 1.0f);
            glVertex2d(0,0);glVertex3d(normalvect[0],normalvect[1],normalvect[2]);
        glEnd();*/
        glRotated(angle, normalvect[0],normalvect[1],normalvect[2]);
    }
    //printCube();
    drawCircleArc(1.0d,0.0d,0.0d, 0.0d,0.0d,1.0d, 1.0d,0.0d,0.0d, 2*M_PI);
    glColor3f(1.0f, 0.0f, 0.0f);
    drawSphere(1.0d,0.0d,0.0d,1.0d, GLU_POINT);
    printAxis(2.);

    glPushMatrix();
}
