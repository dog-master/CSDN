#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>


class Widget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    Widget(QOpenGLWidget *parent = nullptr);
    void makeObject();
    ~Widget();
protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL();
private:
    void creataData( int count , int number);
private:

    QOpenGLShader * flyTrack_vshader;
    QOpenGLShader * flyTrack_fshader;
    QOpenGLShaderProgram* flyTrack_shaderprogram;

    QOpenGLTexture* flyTrack_texture;
    QOpenGLBuffer fly_Track_vbo;
    QImage * flyTrack_image;
    QVector<GLfloat> vertData;


};

#endif // WIDGET_H
