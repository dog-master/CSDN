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


    flyTrack_vshader = new QOpenGLShader(QOpenGLShader::Vertex,this);
    const char *flyTrack_vsrc =
                "attribute highp vec3 vertex;\n"
                "attribute mediump vec2 texCoord;\n"
                "varying mediump vec2 texc;\n"
                "void main(void)\n"
                "{\n"
                "    gl_Position = vec4(vertex,1.0);\n"
                "    texc = texCoord;\n"
                "}\n";
    flyTrack_vshader->compileSourceCode(flyTrack_vsrc);

    flyTrack_fshader = new QOpenGLShader(QOpenGLShader::Fragment,this);
    const char *flyTrack_fsrc =
            "uniform sampler2D texture;\n"
            "varying mediump vec2 texc;\n"
            "void main(void)\n"
            "{\n"
            "    gl_FragColor = texture2D(texture, texc.st);\n"
            "}\n";

    flyTrack_fshader->compileSourceCode(flyTrack_fsrc);

    flyTrack_shaderprogram = new QOpenGLShaderProgram();
    flyTrack_shaderprogram->addShader(flyTrack_vshader);
    flyTrack_shaderprogram->addShader(flyTrack_fshader);
    flyTrack_shaderprogram->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    flyTrack_shaderprogram->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    flyTrack_shaderprogram->link();
    flyTrack_shaderprogram->bind();

    flyTrack_texture = new QOpenGLTexture(QImage(QString(":./wall.jpg")).mirrored());

    fly_Track_vbo.create();
    fly_Track_vbo.bind();
    fly_Track_vbo.allocate(verties,sizeof(verties));

}


void Widget::paintGL()
{


    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    flyTrack_shaderprogram->bind();
    fly_Track_vbo.bind();
    flyTrack_texture->bind();
    flyTrack_shaderprogram->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    flyTrack_shaderprogram->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    flyTrack_shaderprogram->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    flyTrack_shaderprogram->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    glDrawArrays(GL_TRIANGLES,0,3);

    flyTrack_shaderprogram->release();
    fly_Track_vbo.release();
    flyTrack_texture->release();

}


void Widget::resizeGL()
{
    glViewport(0, 0, width(), height());
}



