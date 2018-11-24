#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget * parent) : QOpenGLWidget(parent)
{


    tempFileModel = tempDir.path();
    if (tempDir.isValid()) {
      tempFileModel = tempDir.path();
      QFile::copy(":3d/3d/star.off", tempFileModel + "star.off");
      QFile::copy(":3d/3d/cubo.off", tempFileModel + "cubo.off");
    }
}

void OpenGLWidget::changeShader(int shaderIndex)
{
    if (!model)
        return;

    model->shaderIndex = shaderIndex;
    update();
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
   if(!model)
       return;

   model->zoom += 0.001 * event->delta();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug("OpenGL version: %s", glGetString(GL_VERSION));
    qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST);

    connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer.start(0);

    showFileOpenDialog();
}

void OpenGLWidget::paintGL()
{
    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    efeitosVisuais(atirador);
    efeitosVisuais()

    model->drawModel();
}

void OpenGLWidget:: efeitosVisuais(std::shared_ptr<Model> &m)
{
    int shaderProgramID = m->shaderProgram[m->shaderIndex];
    QVector4D ambientProduct = light.ambient * m->material.ambient;
    QVector4D diffuseProduct = light.diffuse * m->material.diffuse;
    QVector4D specularProduct = light.specular * m->material.specular;

    GLuint locProjection = glGetUniformLocation(shaderProgramID, "projection");
    GLuint locView = glGetUniformLocation(shaderProgramID, "view");
    GLuint locLightPosition = glGetUniformLocation(shaderProgramID, "lightPosition");
    GLuint locAmbientProduct = glGetUniformLocation(shaderProgramID, "ambientProduct");
    GLuint locDiffuseProduct = glGetUniformLocation(shaderProgramID, "diffuseProduct");
    GLuint locSpecularProduct = glGetUniformLocation(shaderProgramID, "specularProduct");
    GLuint locShininess = glGetUniformLocation(shaderProgramID, "shininess");

    glUseProgram(shaderProgramID);

    glUniformMatrix4fv(locProjection, 1, GL_FALSE, camera.projectionMatrix.data());
    glUniformMatrix4fv(locView, 1, GL_FALSE, camera.viewMatrix.data());
    glUniform4fv(locLightPosition, 1, &(light.position[0]));
    glUniform4fv(locAmbientProduct, 1, &(ambientProduct[0]));
    glUniform4fv(locDiffuseProduct, 1, &(diffuseProduct[0]));
    glUniform4fv(locSpecularProduct, 1, &(specularProduct[0]));
    glUniform1f(locShininess, m->material.shininess);
}

void OpenGLWidget::resizeGL(int width, int height)
{
    camera.resizeViewport(width, height);

    if (model)
        model->trackBall.resizeViewport(width, height);

    update();
}

void OpenGLWidget::showFileOpenDialog(){

    int shaderIndex = 0;
    if(atirador)
        shaderIndex = atirador->shaderIndex;

    atirador = std::make_shared<Model>(this);
    atirador->shaderIndex = shaderIndex;
    atirador->readOFFFile(tempFileModel + "star.off");

    model->trackBall.resizeViewport(width(), height());

    tiro = std::make_shared<Model>(this);
    tiro->shaderIndex = shaderIndex;
    tiro->readOFFFile(tempFileModel + "star.off");

    alvo1 = std::make_shared<Model>(this);
    alvo1->shaderIndex = shaderIndex;
    alvo1->readOFFFile(tempFileModel + "cubo.off");

    alvo2 = std::make_shared<Model>(this);
    alvo2->shaderIndex = shaderIndex;
    alvo2->readOFFFile(tempFileModel + "cubo.off");

    alvo3 = std::make_shared<Model>(this);
    alvo3->shaderIndex = shaderIndex;
    alvo3->readOFFFile(tempFileModel + "cubo.off");

    update();
}

void OpenGLWidget::animate()
{
    update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!model)
        return;

    model->trackBall.mouseMove(event->localPos());
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (!model)
        return;

    if (event->button() & Qt::LeftButton)
        model->trackBall.mousePress(event->localPos());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (!model)
        return;

    if (event->button() & Qt::LeftButton)
        model->trackBall.mouseRelease(event->localPos());
}

// Strong focus is required
void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        QApplication::quit();
    }
}

void OpenGLWidget::toggleBackgroundColor(bool changeBColor)
{
    makeCurrent();

    if (changeBColor)
         glClearColor(0, 0, 0, 1);
    else
         glClearColor(1, 1, 1, 1);

    update();
}
