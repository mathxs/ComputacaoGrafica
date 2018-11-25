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

    boolean acertoAlvo1;
    boolean acertoAlvo2;
    boolean acertoAlvo3;

    std::shared_ptr<Model> atirador;
    std::shared_ptr<Model> alvo1;
    std::shared_ptr<Model> alvo2;
    std::shared_ptr<Model> alvo3;
    std::shared_ptr<Model> pontoReferencia;

    float playerPosYOffset;
    float playerPosXOffset;
    float playerPosZOffset;// Player displacement along Y axis
    float playerPosY; // Current player Y position
    float playerPosX;
    float playerPosZ;

    float targetPosYOffset; // Target displacement along Y axis
    float targetPosY; // Current target Y position
    float targetPosY1;
    float targetPosY2;

public:
    OpenGLWidget(QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    QTimer timer;
    QTime time;

signals:
    void statusBarMessage(QString);
    void enableComboShaders(bool);
    void fim(bool);

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
