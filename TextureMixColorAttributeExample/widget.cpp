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
        -0.5f , -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f , -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f ,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    };


    vshader = new QOpenGLShader(QOpenGLShader::Vertex,this);
    const char * vsrc =
            "attribute highp vec3 vertex;\n"
            "attribute mediump vec2 texCoord;\n"
            "attribute mediump vec3 color;\n"
            "varying mediump vec2 texc;\n"
            "varying mediump vec3 ourColor;\n"
            "void main(void)\n"
            "{\n"
            "    gl_Position = vec4(vertex,1.0);\n"
            "    texc = texCoord;\n"
            "    ourColor = color;\n"
            "}\n";
    vshader->compileSourceCode( vsrc);

    fshader = new QOpenGLShader(QOpenGLShader::Fragment,this);
    const char * fsrc =
            "uniform sampler2D texture;\n"
            "varying mediump vec2 texc;\n"
            "varying mediump vec3 ourColor;\n"
            "void main(void)\n"
            "{\n"
            "    gl_FragColor = texture2D(texture, texc.st) * vec4(ourColor,1.0) ;\n"
            "}\n";

    fshader->compileSourceCode(fsrc);

    shaderprogram = new QOpenGLShaderProgram();
    shaderprogram->addShader( vshader);
    shaderprogram->addShader( fshader);
    shaderprogram->link();
    shaderprogram->bind();

    shaderprogram->bindAttributeLocation("vertex",0);
    shaderprogram->bindAttributeLocation("color",1);
    shaderprogram->bindAttributeLocation("texCoord",3);


    texture = new QOpenGLTexture(QImage(QString(":/wall.jpg")).mirrored());

    vbo.create();
    vbo.bind();
    vbo.allocate(verties,sizeof(verties));

}


void Widget::paintGL()
{


    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    shaderprogram->bind();
    vbo.bind();
    texture->bind();
shaderprogram->enableAttributeArray(0);
shaderprogram->enableAttributeArray(1);
shaderprogram->enableAttributeArray(3);
    shaderprogram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 8 * sizeof(GLfloat));
    shaderprogram->setAttributeBuffer(1, GL_FLOAT,3 * sizeof(GLfloat), 3, 8 * sizeof(GLfloat));
    shaderprogram->setAttributeBuffer(3, GL_FLOAT, 6 * sizeof(GLfloat), 2, 8 * sizeof(GLfloat));
//    shaderprogram->enableAttributeArray(shaderprogram->attributeLocation("vertex"));
//    shaderprogram->enableAttributeArray(shaderprogram->attributeLocation("color"));
//     shaderprogram->enableAttributeArray(shaderprogram->attributeLocation("texCoord"));
//    shaderprogram->setAttributeBuffer(shaderprogram->attributeLocation("vertex"), GL_FLOAT, 0, 3, 8 * sizeof(GLfloat));
//    shaderprogram->setAttributeBuffer(shaderprogram->attributeLocation("color"), GL_FLOAT,3 * sizeof(GLfloat), 3, 8 * sizeof(GLfloat));
//    shaderprogram->setAttributeBuffer(shaderprogram->attributeLocation("texCoord"), GL_FLOAT, 6 * sizeof(GLfloat), 2, 8 * sizeof(GLfloat));

    glDrawArrays(GL_TRIANGLES,0,3);

    shaderprogram->release();
    vbo.release();
    texture->release();

}


void Widget::resizeGL()
{
    glViewport(0, 0, width(), height());
}



