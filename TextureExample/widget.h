#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class Widget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    Widget(QOpenGLWidget *parent = nullptr);
    void makeObject();
    ~Widget() override;
protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL();
private:
    void creataData( int count , int number);
private:

    QOpenGLShader *  vshader;
    QOpenGLShader *  fshader;
    QOpenGLShaderProgram*  shaderprogram;
    QOpenGLTexture*  texture;
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;
    QImage *  image;

};

#endif // WIDGET_H
