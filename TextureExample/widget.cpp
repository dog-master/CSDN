#include "widget.h"
#include <QDebug>
#include <QImage>

#define PROGRAM_VERTEX_ATTRIBUTE   0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1


Widget::Widget(QOpenGLWidget *parent)
    : QOpenGLWidget(parent)
{
}

Widget::~Widget()
{
    makeCurrent();
    vbo.destroy();
    vao.destroy();
    delete shaderprogram;
    delete  texture;
    doneCurrent();
}


void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 0.0);

    static float verties[] =
    {
        -0.5f , -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f , -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f ,  0.5f, 0.0f, 0.0f, 1.0f,

    };


    vshader = new QOpenGLShader(QOpenGLShader::Vertex,this);
    const char * vsrc =
            "attribute highp vec3 vertex;\n"
            "attribute mediump vec2 texCoord;\n"
            "varying mediump vec2 texc;\n"
            "void main(void)\n"
            "{\n"
            "    gl_Position = vec4(vertex,1.0);\n"
            "    texc = texCoord;\n"
            "}\n";
    vshader->compileSourceCode( vsrc);

    fshader = new QOpenGLShader(QOpenGLShader::Fragment,this);
    const char * fsrc =
            "uniform sampler2D texture;\n"
            "varying mediump vec2 texc;\n"
            "void main(void)\n"
            "{\n"
            "    gl_FragColor = texture2D(texture, texc.st);\n"
            "}\n";

    fshader->compileSourceCode( fsrc);

    shaderprogram = new QOpenGLShaderProgram();
    shaderprogram->addShader( vshader);
    shaderprogram->addShader( fshader);
    shaderprogram->link();
    shaderprogram->bind();

    texture = new QOpenGLTexture(QImage(QString(":./wall.jpg")).mirrored());

    vbo.create();
    vbo.bind();
    vbo.allocate(verties,sizeof(verties));

    vao.create();
    vao.bind();
    shaderprogram->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    shaderprogram->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    shaderprogram->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    shaderprogram->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

}


void Widget::paintGL()
{


    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    shaderprogram->bind();
    vao.bind();
    texture->bind();
    glDrawArrays(GL_TRIANGLES,0,3);

    shaderprogram->release();
    vao.release();
    texture->release();

}


void Widget::resizeGL()
{
    glViewport(0, 0, width(), height());
}



