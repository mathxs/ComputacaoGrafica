#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include <QtOpenGL>
#include <QOpenGLExtraFunctions>
#include <memory>

#define estrelas 5000

class OpenGLWidget : public QOpenGLWidget, public QOpenGLExtraFunctions
{
    Q_OBJECT
    GLuint vboVertices = 0;
    GLuint vboIndices = 0;
    GLuint shaderProgram;
    std::unique_ptr<QVector4D []> vertices = nullptr;
    std::unique_ptr<unsigned int[]> indices = nullptr;

    QVector3D posicObjet[estrelas]; // Position of each star in NDC

    QTimer timer;
    QTime time;

public:
    OpenGLWidget(QWidget *parent = nullptr);

    void createVBOs();          //feito
    void createShaders();       //feito
    void destroyVBOs();         //feito
    void destroyShaders();      //feito

protected:
    void initializeGL();        //feito
    void paintGL();             //feito

public slots:
    void animate();             //feito
};

#endif // OPENGLWIDGET_H
