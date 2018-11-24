#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget * parent) : QOpenGLWidget(parent)
{
    tempFileModel = tempDir.path();
    if (tempDir.isValid()) {
      tempFileModel = tempDir.path();
      QFile::copy(":3d/3d/star.off", tempFileModel + "star.off");
      QFile::copy(":3d/3d/cubo.off", tempFileModel + "cubo.off");
    }

    playerPosYOffset = 0;
    playerPosY = 0;

    targetPosYOffset = 2.0f;
    targetPosY = 0;
    targetPosY1 = -0.2;
    targetPosY2 = +0.2;

    shooting = false;
    projectilePosX = 0;
    projectilePosY = 0;

    numHits = 0;
}

void OpenGLWidget::changeShader(int shaderIndex)
{
    if (tiro)
        atirador->shaderIndex = shaderIndex;
        tiro->shaderIndex = shaderIndex;
        alvo1->shaderIndex = shaderIndex;
        alvo2->shaderIndex = shaderIndex;
        alvo3->shaderIndex = shaderIndex;
    update();
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
   if(!atirador)
       return;

   atirador->zoom += 0.001 * event->delta();
   tiro->zoom += 0.001 * event->delta();
   alvo1->zoom += 0.001 * event->delta();
   alvo2->zoom += 0.001 * event->delta();
   alvo3->zoom += 0.001 * event->delta();
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
    efeitosVisuais(tiro);
    efeitosVisuais(alvo1);
    efeitosVisuais(alvo2);
    efeitosVisuais(alvo3);

    atirador->drawModel(playerPosY, 0.0f, 0.0f, 0.5f,0.5f,0.5f);
    alvo3->drawModel(targetPosY, 1.0f, 0.0f, 0.4f,0.4f,0.4f);
    alvo1->drawModel(targetPosY1, 0.0f, 0.0f, 0.3f,0.3f,0.3f);
    alvo2->drawModel(targetPosY2, -1.0f, 0.0f, 0.3f,0.3f,0.3f);
    if (shooting)
    {
        tiro->drawModel(projectilePosY, projectilePosY, 0.0f, 0.1f,0.1f,0.1f);
    }



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

    if (tiro)
        atirador->trackBall.resizeViewport(width, height);
        tiro->trackBall.resizeViewport(width, height);
        alvo1->trackBall.resizeViewport(width, height);
        alvo2->trackBall.resizeViewport(width, height);
        alvo3->trackBall.resizeViewport(width, height);
    update();
}

void OpenGLWidget::showFileOpenDialog(){

    int shaderIndex = 0;
    if(atirador)
        shaderIndex = atirador->shaderIndex;

    atirador = std::make_shared<Model>(this);
    atirador->shaderIndex = shaderIndex;
    //atirador->readOFFFile(tempFileModel + "star.off");
    atirador->readOFFFile("C:/Users/Matheus/Documents/ArqDesen/RespositorioGit/ComputacaoGrafica/ProjetoGC/3d/star.off");

    tiro = std::make_shared<Model>(this);
    tiro->shaderIndex = shaderIndex;
    //tiro->readOFFFile(tempFileModel + "star.off");
    tiro->readOFFFile("C:/Users/Matheus/Documents/ArqDesen/RespositorioGit/ComputacaoGrafica/ProjetoGC/3d/star.off");

    alvo1 = std::make_shared<Model>(this);
    alvo1->shaderIndex = shaderIndex;
    //alvo1->readOFFFile(tempFileModel + "cubo.off");
    alvo1->readOFFFile("C:/Users/Matheus/Documents/ArqDesen/RespositorioGit/ComputacaoGrafica/ProjetoGC/3d/cubo.off");

    alvo2 = std::make_shared<Model>(this);
    alvo2->shaderIndex = shaderIndex;
    //alvo2->readOFFFile(tempFileModel + "cubo.off");
    alvo2->readOFFFile("C:/Users/Matheus/Documents/ArqDesen/RespositorioGit/ComputacaoGrafica/ProjetoGC/3d/cubo.off");

    alvo3 = std::make_shared<Model>(this);
    alvo3->shaderIndex = shaderIndex;
    //alvo3->readOFFFile(tempFileModel + "cubo.off");
    alvo3->readOFFFile("C:/Users/Matheus/Documents/ArqDesen/RespositorioGit/ComputacaoGrafica/ProjetoGC/3d/cubo.off");

    atirador->trackBall.resizeViewport(width(), height());
    update();
}

void OpenGLWidget::animate()
{
    float elapsedTime = time.restart() / 1000.0f;

    // Change player Y position
    playerPosY += playerPosYOffset * elapsedTime;

    // Check player bounds
    if (playerPosY < -0.8f)
        playerPosY = -0.8f;
    if (playerPosY > 0.8f)
        playerPosY = 0.8f;

    // Update target
    targetPosY += targetPosYOffset * elapsedTime;
    targetPosY1 += targetPosYOffset * elapsedTime;
    targetPosY2 += targetPosYOffset * elapsedTime;

    if (targetPosYOffset > 0)
    {
        if (targetPosY > 0.8f)
        {
            targetPosY = 0.8f;
            targetPosY1 = 0.6f;
            targetPosY2 = 1.0f;
            targetPosYOffset = -targetPosYOffset;
        }
    }
    else if (targetPosYOffset < 0)
    {
        if (targetPosY < -0.8f)
        {
            targetPosY = -0.8f;
            targetPosY1 = -1.0f;
            targetPosY2 = -0.6f;
            targetPosYOffset = -targetPosYOffset;
        }
    }

    // Update projectile
    if (shooting)
    {
        // Move projectile
        projectilePosX += 3.0f * elapsedTime;

        // Check whether the projectile hit the target
        if(projectilePosX > 0.9f)
            {
                if (std::fabs(projectilePosY - targetPosY1) < 0.125f || std::fabs(projectilePosY - targetPosY2) < 0.125f)
                {
                    numHits++;
                    qDebug("Hit!");
                    shooting = false;
                }

            }
        else if (projectilePosX > 0.5f)
        {
            if (std::fabs(projectilePosY - targetPosY) < 0.125f)
            {
                numHits = numHits -1;
                qDebug("Hit!");
                shooting = false;
            }
        }
    }
    // Check whether the projectile missed the target
    if (projectilePosX > 1.0f)
    {
        qDebug("Missed");
        shooting = false;
    }
    update();

}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!atirador)
        return;

    atirador->trackBall.mouseMove(event->localPos());
    tiro->trackBall.mouseMove(event->localPos());
    alvo1->trackBall.mouseMove(event->localPos());
    alvo2->trackBall.mouseMove(event->localPos());
    alvo3->trackBall.mouseMove(event->localPos());
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (!atirador)
        return;

    if (event->button() & Qt::LeftButton)
        atirador->trackBall.mousePress(event->localPos());
        tiro->trackBall.mousePress(event->localPos());
        alvo1->trackBall.mousePress(event->localPos());
        alvo2->trackBall.mousePress(event->localPos());
        alvo3->trackBall.mousePress(event->localPos());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (!atirador)
        return;

    if (event->button() & Qt::LeftButton)
        atirador->trackBall.mouseRelease(event->localPos());
        tiro->trackBall.mouseRelease(event->localPos());
        alvo1->trackBall.mouseRelease(event->localPos());
        alvo2->trackBall.mouseRelease(event->localPos());
        alvo3->trackBall.mouseRelease(event->localPos());
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right ||
        event->key() == Qt::Key_D)
        playerPosYOffset = 2.0f;

    if (event->key() == Qt::Key_Left ||
        event->key() == Qt::Key_A)
        playerPosYOffset = -2.0f;

    if (event->key() == Qt::Key_Space)
    {
        if (!shooting)
        {
            shooting = true;
            projectilePosX = -0.7f;
            projectilePosY = playerPosY;
        }
    }

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
