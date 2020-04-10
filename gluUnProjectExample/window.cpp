#include "window.h"
#include <QDebug>
#include <QString>
#include <GL/glu.h>
#include <QPainter>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include "Vertex.h"

// Create a colored triangle
static const Vertex sg_vertexes[] = {
  Vertex( QVector3D( 0.00f,  0.75f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f) ),
  Vertex( QVector3D( 0.75f, -0.75f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f) ),
  Vertex( QVector3D(-0.75f, -0.75f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f) )
};



void Window::initializeGL()
{
  // Initialize OpenGL Backend
  initializeOpenGLFunctions();
  printVersionInformation();

  // Set global information
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // Application-specific initialization
  {
    // Create Shader (Do not release until VAO is created)
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
    m_program->link();
    m_program->bind();

    // Create Buffer (Do not release until VAO is created)
    m_vertex.create();
    m_vertex.bind();
    m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertex.allocate(sg_vertexes, sizeof(sg_vertexes));

    // Create Vertex Array Object
    m_object.create();
    m_object.bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
    m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

    // Release (unbind) all
    m_object.release();
    m_vertex.release();
    m_program->release();
  }
}


void Window::paintGL()
{
  QPainter painter;
  painter.begin(this);
    // Clear
  glClear(GL_COLOR_BUFFER_BIT);

  // Render using our shader
  m_program->bind();
  {
    m_object.bind();
    glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));
    m_object.release();
  }
  m_program->release();

   GLint    viewport[4];
   GLdouble modelview[16];
   GLdouble projection[16];

    painter.endNativePainting();
   painter.setPen(QColor(0xff,0xff,0xff));

   painter.drawText(QPoint(this->winX,this->winY),QString("X:%1,Y:%2").arg(this->posX).arg(this->posY));


   glGetIntegerv(GL_VIEWPORT, viewport); // 得到的是最后一个设置视口的参数
   glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
   glGetDoublev(GL_PROJECTION_MATRIX, projection);


//   glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
   gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

   qDebug()<<"posX"<<posX<<"posY"<<posY<<"posZ"<<posZ;
}

void Window::resizeGL(int width, int height)
{
  // Currently we are not handling width/height changes.
  (void)width;
  (void)height;
}



void Window::teardownGL()
{
  // Actually destroy our OpenGL information
  m_object.destroy();
  m_vertex.destroy();
  delete m_program;
}

/*******************************************************************************
 * Private Helpers
 ******************************************************************************/

void Window::printVersionInformation()
{
  QString glType;
  QString glVersion;
  QString glProfile;

  // Get Version Information
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

  // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
  switch (format().profile())
  {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

  // qPrintable() will print our QString w/o quotes around it.
  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}


void Window::mouseMoveEvent(QMouseEvent *event)
{
    this->winX = event->pos().x();
    this->winY = event->pos().y();
    this->winZ = 0;
    qDebug()<<"winX:"<<this->winX<<"winY"<<this->winY;
    update();

}
