#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>


#include "light.h"
#include "model.h"
#include "camera.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

    QString tempFileModel;
    QTemporaryDir tempDir;

    std::shared_ptr<Model> atirador;
    std::shared_ptr<Model> tiro;
    std::shared_ptr<Model> alvo1;
    std::shared_ptr<Model> alvo2;
    std::shared_ptr<Model> alvo3;

public:
    OpenGLWidget(QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    QTimer timer;

signals:
    void statusBarMessage(QString);
    void enableComboShaders(bool);

public slots:
    void showFileOpenDialog();
    void changeShader(int);
    void animate();
    void toggleBackgroundColor(bool);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void efeitosVisuais(std::shared_ptr<Model> &m);

    Light light;
    Camera camera;

    void updateScene(int);
};

#endif // OPENGLWIDGET_H
