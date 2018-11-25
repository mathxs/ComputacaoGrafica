#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget * parent) : QOpenGLWidget(parent)
{
    tempFileModel = tempDir.path();
    if (tempDir.isValid()) {
      tempFileModel = tempDir.path();
      QFile::copy(":/3d/3d/star.off", tempFileModel + "star.off");
      QFile::copy(":/3d/3d/cubo.off", tempFileModel + "cubo.off");
      QFile::copy(":/3d/3d/piso.off", tempFileModel + "piso.off");
    }

    playerPosYOffset = 0;
    playerPosXOffset = 0;
    playerPosZOffset = 0;

    playerPosY = 0.0f;
    playerPosX = 0.0f;
    playerPosZ = 0.0f;

    targetPosYOffset = 2.0f;
    targetPosY = 0;
    targetPosY1 = -10.0;
    targetPosY2 = 10.0;

    acertoAlvo1 = false;
    acertoAlvo2 = false;
    acertoAlvo3 = false;
    emit fim(false);
    contador = 0;

}

void OpenGLWidget::changeShader(int shaderIndex)
{
        atirador->shaderIndex = shaderIndex;
        alvo1->shaderIndex = shaderIndex;
        alvo2->shaderIndex = shaderIndex;
        alvo3->shaderIndex = shaderIndex;
        pontoReferencia->shaderIndex = shaderIndex;
    update();
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{

   //pontoReferencia->zoom += 0.001 * event->delta();
   atirador->zoom += 0.001 * event->delta();
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

    atirador->drawModel(playerPosY, playerPosX, playerPosZ, 0.5f,0.5f,0.5f);
    efeitosVisuais(atirador);

    if(!acertoAlvo3){
        alvo3->drawModel(targetPosY, 12.0f, 0.0f, 0.4f,0.4f,0.4f);
        efeitosVisuais(alvo3);
    }

    if(!acertoAlvo1){
        alvo1->drawModel(targetPosY1, 12.0f, 0.0f, 0.4f,0.4f,0.4f);
        efeitosVisuais(alvo1);
    }

    if(!acertoAlvo2){
        alvo2->drawModel(targetPosY2, 12.0f, 0.0f, 0.4f,0.4f,0.4f);
        efeitosVisuais(alvo2);
    }

    if(acertoAlvo1 and acertoAlvo2 and acertoAlvo3){
        emit fim(true);
        qDebug("FIM");
        contador += 1;
        if(contador > 10){
            Sleep(1000);
            qApp->exit(0);
        }
    }else{
        emit fim(false);
    }

    //efeitosVisuais(pontoReferencia);
    //pontoReferencia->drawModel(-1.0f, 0.0f, 0.0f, 100.0f,100.0f,1.0f);


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

        atirador->trackBall.resizeViewport(width, height);
        alvo1->trackBall.resizeViewport(width, height);
        alvo2->trackBall.resizeViewport(width, height);
        alvo3->trackBall.resizeViewport(width, height);
        //pontoReferencia->trackBall.resizeViewport(width, height);
    update();
}

void OpenGLWidget::showFileOpenDialog(){

    int shaderIndex = 0;
    if(atirador)
        shaderIndex = atirador->shaderIndex;

    atirador = std::make_shared<Model>(this);
    atirador->shaderIndex = shaderIndex;
    atirador->readOFFFile(tempFileModel + "star.off");
    loadTex(":/3d/3d/vermelho.png", atirador);
    //atirador->readOFFFile("C:/Users/Matheus/Documents/ArqDesen/RespositorioGit/ComputacaoGrafica/ProjetoGC/3d/star.off");

    alvo1 = std::make_shared<Model>(this);
    alvo1->shaderIndex = shaderIndex;
    alvo1->readOFFFile(tempFileModel + "cubo.off");
    loadTex(":/3d/3d/amarelo.png", alvo1);
    //alvo1->readOFFFile("C:/Users/Matheus/Documents/ArqDesen/RespositorioGit/ComputacaoGrafica/ProjetoGC/3d/cubo.off");

    alvo2 = std::make_shared<Model>(this);
    alvo2->shaderIndex = shaderIndex;
    alvo2->readOFFFile(tempFileModel + "cubo.off");
    loadTex(":/3d/3d/cinza.png", alvo2);
    //alvo2->readOFFFile("C:/Users/Matheus/Documents/ArqDesen/RespositorioGit/ComputacaoGrafica/ProjetoGC/3d/cubo.off");

    alvo3 = std::make_shared<Model>(this);
    alvo3->shaderIndex = shaderIndex;
    alvo3->readOFFFile(tempFileModel + "cubo.off");
    loadTex(":/3d/3d/roxo.png", alvo3);
    //alvo3->readOFFFile("C:/Users/Matheus/Documents/ArqDesen/RespositorioGit/ComputacaoGrafica/ProjetoGC/3d/cubo.off");

    pontoReferencia = std::make_shared<Model>(this);
    pontoReferencia->shaderIndex = shaderIndex;
    pontoReferencia->readOFFFile(tempFileModel + "piso.off");
    //pontoReferencia->readOFFFile("C:/Users/Matheus/Documents/ArqDesen/RespositorioGit/ComputacaoGrafica/ProjetoGC/3d/piso.off");

    atirador->trackBall.resizeViewport(width(), height());
    update();
}

void OpenGLWidget::animate()
{
    float elapsedTime = time.restart() / 1000.0f;

    // Change player Y position
    playerPosY += playerPosYOffset * elapsedTime;
    playerPosX += playerPosXOffset * elapsedTime;
    playerPosZ += playerPosZOffset * elapsedTime;

    // Update target
    targetPosY += targetPosYOffset * 5 * elapsedTime;
    targetPosY1 += targetPosYOffset * 5 * elapsedTime;
    targetPosY2 += targetPosYOffset * 5 * elapsedTime;

    if (targetPosYOffset > 0)
    {
        if (targetPosY > 20.0f)
        {
            targetPosY = 20.0f;
            targetPosY1 = 15.0f;
            targetPosY2 = 25.0f;
            targetPosYOffset = -targetPosYOffset;
        }
    }
    else if (targetPosYOffset < 0)
    {
        if (targetPosY < -20.0f)
        {
            targetPosY = -20.0f;
            targetPosY1 = -15.0f;
            targetPosY2 = -25.0f;
            targetPosYOffset = -targetPosYOffset;
        }
    }
    qDebug("t1:%f t2:%f t3:%f X:%f Y:%f Z:%f",targetPosY1,targetPosY,targetPosY2,playerPosY,playerPosX,playerPosZ);

    if ( (abs(playerPosX)>8.0f) and (abs(playerPosX)<12.0f) and (abs(playerPosZ)>5.0f) and (abs(playerPosZ)<7.0f) ){
         qDebug("alinhado!");
        if((abs(abs(targetPosY1) -  abs(playerPosY))<1.5f))
         {
               acertoAlvo1 = true;
               qDebug("Hit alvo1!");
         }

        if((abs(abs(targetPosY2) -  abs(playerPosY))<1.5f))
         {
               acertoAlvo2 = true;
               qDebug("Hit alvo2!");
         }

        if((abs(abs(targetPosY) -  abs(playerPosY))<1.5f))
         {
               acertoAlvo3 = true;
               qDebug("Hit! alvo3");
         }
    }
    update();

}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!atirador)
        return;

    atirador->trackBall.mouseMove(event->localPos());
    alvo1->trackBall.mouseMove(event->localPos());
    alvo2->trackBall.mouseMove(event->localPos());
    alvo3->trackBall.mouseMove(event->localPos());
    pontoReferencia->trackBall.mouseMove(event->localPos());
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton)
        atirador->trackBall.mousePress(event->localPos());
        alvo1->trackBall.mousePress(event->localPos());
        alvo2->trackBall.mousePress(event->localPos());
        alvo3->trackBall.mousePress(event->localPos());
        pontoReferencia->trackBall.mousePress(event->localPos());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton)
        atirador->trackBall.mouseRelease(event->localPos());
        alvo1->trackBall.mouseRelease(event->localPos());
        alvo2->trackBall.mouseRelease(event->localPos());
        alvo3->trackBall.mouseRelease(event->localPos());
        pontoReferencia->trackBall.mousePress(event->localPos());
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right ||
        event->key() == Qt::Key_D)
        playerPosYOffset = 2.0f;

    if (event->key() == Qt::Key_Left ||
        event->key() == Qt::Key_A)
        playerPosYOffset = -2.0f;

    if (event->key() == Qt::Key_Up ||
        event->key() == Qt::Key_W)
        playerPosXOffset = 2.0f;

    if (event->key() == Qt::Key_Down ||
        event->key() == Qt::Key_S)
        playerPosXOffset = -2.0f;

    if (event->key() == Qt::Key_0)
    {
        playerPosXOffset = 0.0f;
        playerPosYOffset = 0.0f;
        playerPosZOffset = 0.0f;
    }

    if (event->key() == Qt::Key_Space)
    {
        acertoAlvo1 = true;
        acertoAlvo2 = true;
        acertoAlvo3 = true;
    }

    if (event->key() == Qt::Key_P)
    {
        Sleep(10000);
    }

    if (event->key() == Qt::Key_1)
    {
        playerPosZOffset = 2.0f;
    }

    if (event->key() == Qt::Key_2)
    {
        playerPosZOffset = -2.0f;
    }
    if (event->key() == Qt::Key_Escape)
    {
        QApplication::quit();
    }
}

void OpenGLWidget::loadTex(QString const &fileName, std::shared_ptr<Model> &m){

    QImage image1;
    image1.load(fileName);
    image1 = image1.convertToFormat(QImage::Format_RGBA8888);
    m->loadTexture(image1);

    update ();
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
