#include "openglwidgetativ3.h"
#include <QMediaPlayer>
#include <QFileDialog>

void OpenGLWidget :: musica()
{

    media = new QMediaPlayer(this);
    media->setMedia(QUrl::fromLocalFile("qrc:/shaders/faixa1.mp4"));
    media->setVolume(30);
    media->play();

}

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
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

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    qDebug("OpenGL version: %s", glGetString(GL_VERSION));
    qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    createShaders();
    createVBOs();

    connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer.start(0);

    time.start();
    //musica();
}

void OpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint locScaling = glGetUniformLocation(shaderProgram, "scaling");
    GLuint locTranslation = glGetUniformLocation(shaderProgram, "translation");

    glUseProgram(shaderProgram);

    glBindVertexArray(vao);

    // Player
    glUniform4f(locTranslation, playerPosY,-0.8, 0, 0);
    glUniform1f(locScaling, 0.15);
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, 0);

    // Target
    glUniform4f(locTranslation, targetPosY, 0.5, 0, 0);
    glUniform1f(locScaling, 0.11);
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, 0);

    // Target2
    glUniform4f(locTranslation, targetPosY1, 0.9, 0, 0);
    glUniform1f(locScaling, 0.08);
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, 0);

    // Target2
    glUniform4f(locTranslation, targetPosY2, 0.9, 0, 0);
    glUniform1f(locScaling, 0.08);
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, 0);

    // Projectile
    if (shooting)
    {
        glUniform4f(locTranslation, projectilePosY, projectilePosX, 0, 0);
        glUniform1f(locScaling, 0.05);
        glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, 0);
    }
}

void OpenGLWidget::createVBOs()
{
    makeCurrent();
    destroyVBOs();

    vertices = std::make_unique<QVector4D[]>(4);
    colors = std::make_unique<QVector4D[]>(4);
    indices = std::make_unique<unsigned int[]>(2 * 3);

    vertices[0] = QVector4D( 0.0, -1.00, 0, 1);
    vertices[1] = QVector4D(-0.75,  0.5, 0, 1);
    vertices[2] = QVector4D( 0.75,  0.5, 0, 1);
    vertices[3] = QVector4D( 0.0,  1.00, 0, 1);
    vertices[4] = QVector4D(-0.75, -0.5, 0, 1);
    vertices[5] = QVector4D( 0.75, -0.5, 0, 1);

    colors[0] = QVector4D(1, 0, 0, 1); // red
    colors[1] = QVector4D(0, 1, 0, 1); // green
    colors[2] = QVector4D(0, 0, 1, 1); // blue
    colors[3] = QVector4D(1, 1, 0, 1); // yellow

    // Topology of the mesh (square)
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 3;
    indices[4] = 4;
    indices[5] = 5;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(QVector4D), vertices.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vboColors);
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(QVector4D), colors.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indices.get(), GL_DYNAMIC_DRAW);

}

void OpenGLWidget::createShaders()
{
    destroyShaders();

    QFile vs(":/shaders/vshader.glsl");
    QFile fs(":/shaders/fshader.glsl");

    vs.open(QFile::ReadOnly | QFile::Text);
    fs.open(QFile::ReadOnly | QFile::Text);

    QTextStream streamVs(&vs), streamFs(&fs);

    QString qtStringVs = streamVs.readAll();
    QString qtStringFs = streamFs.readAll();

    std::string stdStringVs = qtStringVs.toStdString();
    std::string stdStringFs = qtStringFs.toStdString();

    // Create an empty vertex shader handle
    GLuint vertexShader = 0;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    const GLchar *source = stdStringVs.c_str();

    glShaderSource(vertexShader, 1, &source, nullptr);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        qDebug("%s", &infoLog[0]);

        glDeleteShader(vertexShader);
        return;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = 0;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = stdStringFs.c_str();
    glShaderSource(fragmentShader, 1, &source, nullptr);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        qDebug("%s", &infoLog[0]);

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    shaderProgram = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Link our program
    glLinkProgram(shaderProgram);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
        qDebug("%s", &infoLog[0]);

        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    vs.close();
    fs.close();
}

void OpenGLWidget::reconstruindoCores()
{
    destroyShaders();
    destroyVBOs();
    createShaders();
    createVBOs();
}

void OpenGLWidget::destroyVBOs()
{
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboColors);
    glDeleteBuffers(1, &vboIndices);
    glDeleteVertexArrays(1, &vao);

    vboVertices = 0;
    vboIndices = 0;
    vboColors = 0;
    vao = 0;
}

void OpenGLWidget::destroyShaders()
{
    makeCurrent();
    glDeleteProgram(shaderProgram);
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
            targetPosY1 = 1.0f;
            targetPosY2 = 0.6f;
            targetPosYOffset = -targetPosYOffset;
        }
    }
    else if (targetPosYOffset < 0)
    {
        if (targetPosY < -0.8f)
        {
            targetPosY = -0.8f;
            targetPosY1 = -0.6f;
            targetPosY2 = -1.0f;
            targetPosYOffset = -targetPosYOffset;
        }
    }

    // Update projectile
    if (shooting)
    {
        // Move projectile
        projectilePosX += 3.0f * elapsedTime;

        // Check whether the projectile hit the target
        if (projectilePosX > 0.5f)
        {
            if (std::fabs(projectilePosY - targetPosY) < 0.125f)
            {
                numHits++;
                qDebug("Hit!");
                emit updateHitsLabel(QString("Pontos: %1").arg(numHits));
                shooting = false;
                reconstruindoCores();
            }
        }
        else if(projectilePosX > 0.9f)
            {
                if (std::fabs(projectilePosY - targetPosY1) < 0.125f)
                {
                    numHits++;
                    qDebug("Hit!");
                    emit updateHitsLabel(QString("Pontos: %1").arg(numHits));
                    shooting = false;
                    reconstruindoCores();
                }
            }
         else if(projectilePosX > 0.9f)
            {
                if (std::fabs(projectilePosY - targetPosY2) < 0.125f)
                {
                    numHits++;
                    qDebug("Hit!");
                    emit updateHitsLabel(QString("Pontos: %1").arg(numHits));
                    shooting = false;
                    reconstruindoCores();
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

    if (event->key() == Qt::Key_F1)
    {
        reconstruindoCores();
    }
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right ||
        event->key() == Qt::Key_D)
        playerPosYOffset = 0;

    if (event->key() == Qt::Key_Left ||
        event->key() == Qt::Key_A)
        playerPosYOffset = 0;
}
