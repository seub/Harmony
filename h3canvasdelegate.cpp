#include "h3canvasdelegate.h"


std::ostream & operator<<(std::ostream & out, const GLVector &v)
{
    out << "x :" << v.x << ", y : " << v.y << ", z : " << v.z;
    return out;
}


void operator +=(GLVector & v, const GLVector & w)
{
    v = v.add(w);
}

GLVector operator -(const GLVector & v)
{
    return v.opposite();
}

void operator -=(GLVector & v, const GLVector & w)
{
    v = v.add(-w);
}

void operator *=(GLVector & w, const GLdouble & v)
{
    w = w.scalarMult(v);
}

GLdouble operator *(const GLVector & v, const GLVector & w)
{
    return v.scalarProduct(w);
}

GLVector operator *(const GLdouble & v, const GLVector & w)
{
    return w.scalarMult(v);
}

GLVector operator ^(const GLVector & v, const GLVector & w)
{
    return v.vectorProduct(w);
}

GLVector operator +(const GLVector & v, const GLVector & w)
{
    return v.add(w);
}

GLVector operator -(const GLVector & v, const GLVector & w)
{
    return v.add(-w);
}

GLVector::GLVector(GLdouble x, GLdouble y, GLdouble z):x(x),y(y),z(z)
{

}

GLdouble GLVector::scalarProduct(const GLVector &v) const
{
    return x*v.x +y*v.y +z*v.z;
}

GLVector GLVector::vectorProduct(const GLVector &v) const
{
    return GLVector(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
}

GLVector GLVector::add(const GLVector &v) const
{
    return GLVector(x+v.x,y+v.y,z+v.z);
}

GLVector GLVector::opposite() const
{
    return GLVector(-x,-y,-z);
}

GLVector GLVector::scalarMult(const GLdouble lambda) const
{
    return GLVector(lambda*x,lambda*y,lambda*z);
}


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

void H3canvasDelegate::glVertex3(const GLVector v)
{
    glVertex3d(v.x,v.y,v.z);
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


void H3canvasDelegate::mouseGLCoordinates(const QMouseEvent *mouseevent, GLVector &pos, GLdouble *modelView) const
{
    bool createModelView = false;
    GLint viewport[4];
    GLdouble projection[16];
    if(modelView == 0)
    {
        createModelView = true;
        modelView = new GLdouble[16];
        glGetDoublev( GL_MODELVIEW_MATRIX, modelView);
    }
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluProject(0.0d,0.0d,0.0d,modelView,projection,viewport,&pos.x,&pos.y,&pos.z);
    gluUnProject(mouseevent->x(),viewport[3]- mouseevent->y(),pos.z,modelView,projection,viewport,&pos.x,&pos.y,&pos.z);
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
    glRotated(angle, normalvect.x,normalvect.y,normalvect.z);
    glPushMatrix();

}

void H3canvasDelegate::mouseMoveEvent(QMouseEvent * mouseevent)
{
    if(isClicked)
    {
        mouseGLCoordinates(mouseevent, currentPosition, modelviewsave);
        normalvect = mousePosition^currentPosition;


        if(mousePosition.norm() > currentPosition.norm())
        {
            GLdouble lambda[2];
            lambda[0] =(sqrt((mousePosition.norm()*mousePosition.norm()
                              - currentPosition.norm()*currentPosition.norm())))/normalvect.norm();
            lambda[1] = -lambda[0];

                GLint viewport[4];
                GLdouble projection[16];
                glGetDoublev(GL_PROJECTION_MATRIX, projection);
                glGetIntegerv(GL_VIEWPORT, viewport);
                GLdouble x,y,z[2];
                GLVector temp = currentPosition + lambda[0]*normalvect;
                gluProject(temp.x,temp.y,temp.z,modelviewsave,projection,viewport,&x,&y,z);
                temp = currentPosition + lambda[1]*normalvect;
                gluProject(temp.x,temp.y,temp.z,modelviewsave,projection,viewport,&x,&y,z+1);
                std::cout << lambda[0] << " " << lambda[1] << std::endl;
                int i = z[0]<z[1]?0:1;
                currentPosition += lambda[i]*normalvect;
                normalvect = mousePosition^currentPosition;
        }
        else
        {
            mousePosition*=currentPosition.norm()/mousePosition.norm();
        }
        //angle = 180*asin(norNorm/(curNorm*posNorm))/M_PI*100;
        angle = 180*acos((mousePosition*currentPosition)/(mousePosition.norm()*currentPosition.norm()))/M_PI;
        if(angle!=angle) angle =0;
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

void H3canvasDelegate::drawSphere(const GLVector center, GLdouble radius , GLenum style)
{
    GLUquadricObj *quadric;
    quadric = gluNewQuadric();
    glTranslated(center.x, center.y, center.z);
    gluQuadricDrawStyle(quadric, style );

    gluSphere( quadric , radius , 10, 10 );
    glTranslated(-center.x, -center.y, -center.z);

    gluDeleteQuadric(quadric);
    glEndList();
}

void H3canvasDelegate::drawCircleArc(GLVector center,
                                     GLVector normal,
                                     GLVector first, //relative to the center
                                     GLdouble angle)
{
    angle = angle<2*M_PI?angle:2*M_PI;
    GLVector prod = (1/normal.norm())*normal^first;
    GLVector temp;
    double i;
    glBegin(GL_LINE_STRIP);
        glColor3f(0.0f, 0.0f, 1.0f);
        temp = center+first;
        glVertex3d(temp.x,temp.y,temp.z);
        for(i =0; i < angle; i+=0.01)
        {
            temp = cos(i)*first + sin(i)*prod + center;
            glVertex3d(temp.x,temp.y,temp.z);
        }
        temp = cos(i)*first + sin(i)*prod + center;
        glVertex3d(temp.x,temp.y,temp.z);
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
        glRotated(angle, normalvect.x,normalvect.y,normalvect.z);
    }
    //printCube();
    drawCircleArc(GLVector(1.0d,0.0d,0.0d), GLVector(0.0d,0.0d,1.0d), GLVector(1.0d,0.0d,0.0d), 2*M_PI);
    glColor3f(1.0f, 0.0f, 0.0f);
    drawSphere(GLVector(1.0d,0.0d,0.0d),1.0d, GLU_POINT);
    printAxis(2.);

    glPushMatrix();
}
