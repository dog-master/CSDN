#ifndef WINDOW_H
#define WINDOW_H
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "Vertex.h"

class QOpenGLShaderProgram;

class Window : public QOpenGLWindow,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void teardownGL();
protected:
    void mouseMoveEvent(QMouseEvent *event);
private:
    // OpenGL State Information
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;
    GLdouble  posX, posY, posZ;
    GLdouble winX, winY, winZ;
    // Private Helpers
    void printVersionInformation();
};

#endif // WINDOW_H
