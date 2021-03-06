#include "model.h"

Model::Model(QOpenGLWidget *_glWidget)
{   
    glWidget = _glWidget;
    glWidget->makeCurrent();

    initializeOpenGLFunctions();

    shaderIndex = 0;
    numShaders = 0;
}

Model::~Model()
{
    destroyVBOs();
    destroyShaders();
}

void Model::destroyVBOs()
{
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboIndices);
    glDeleteBuffers(1, &vboNormals);
    glDeleteBuffers(1, &vboTexCoords);
    glDeleteBuffers(1, &vboTangents);

    glDeleteVertexArrays(1, &vao);

    vboVertices = 0;
    vboIndices = 0;
    vboNormals = 0;
    vboTexCoords = 0;
    vboTangents = 0;
    vao = 0;
}

void Model::createVBOs()
{
    glWidget->makeCurrent();

    destroyVBOs();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector4D), vertices.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    vertices.reset();

    glGenBuffers(1, &vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector3D), normals.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);
    normals.reset();

    glGenBuffers(1, &vboTexCoords);
    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector2D), texCoords.get(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(2);
    texCoords.reset();

    glGenBuffers(1, &vboTangents);
    glBindBuffer(GL_ARRAY_BUFFER, vboTangents);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector4D), tangents.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(3);
    tangents.reset();

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(unsigned int), indices.get(), GL_STATIC_DRAW);
    indices.reset();
}

void Model::destroyShaders ()
{
    for (GLuint shaderProgramID : shaderProgram)
    {
        glDeleteProgram(shaderProgramID);
    }
    shaderProgram.clear();
}

void Model::createShaders ()
{
    numShaders = 10;

    QString vertexShaderFile[]  = {":/shaders/shaders/vshader1.glsl",
                                   ":/shaders/shaders/vflat.glsl",
                                   ":/shaders/shaders/vgouraud.glsl",
                                   ":/shaders/shaders/vphong.glsl",
                                   ":/shaders/shaders/vtoon.glsl",
                                   ":/shaders/shaders/vnormal.glsl",
                                   ":/shaders/shaders/vtexture.glsl",
                                   ":/shaders/shaders/vtex2.glsl",
                                   ":/shaders/shaders/vnormalmap.glsl",
                                   ":/shaders/shaders/vcubemap.glsl"};

    QString fragmentShaderFile[] = {":/shaders/shaders/fshader1.glsl",
                                    ":/shaders/shaders/fflat.glsl",
                                    ":/shaders/shaders/fgouraud.glsl",
                                    ":/shaders/shaders/fphong.glsl",
                                    ":/shaders/shaders/ftoon.glsl",
                                    ":/shaders/shaders/fnormal.glsl",
                                    ":/shaders/shaders/ftexture.glsl",
                                    ":/shaders/shaders/ftex2.glsl",
                                    ":/shaders/shaders/fnormalmap.glsl",
                                    ":/shaders/shaders/fcubemap.glsl"};

    destroyShaders();

    shaderProgram.clear();

    for (int i = 0; i < numShaders; ++i)
    {
        QFile vs(vertexShaderFile[i]);
        QFile fs(fragmentShaderFile[i]);

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

        glShaderSource(vertexShader, 1, &source, 0);

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
        source = stdStringFs.c_str();//(const GLchar *)stringFs.toStdString().c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

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
        GLuint shaderProgramID = 0;
        shaderProgramID = glCreateProgram();
        shaderProgram.push_back(shaderProgramID);

        // Attach our shaders to our program
        glAttachShader(shaderProgramID, vertexShader);
        glAttachShader(shaderProgramID, fragmentShader);

        // Link our program
        glLinkProgram(shaderProgramID);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(shaderProgramID, maxLength, &maxLength, &infoLog[0]);
            qDebug("%s", &infoLog[0]);

            glDeleteProgram(shaderProgramID);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return;
        }

        glDetachShader(shaderProgramID, vertexShader);
        glDetachShader(shaderProgramID, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        vs.close();
        fs.close();
    }
}

void Model::drawModel(float posX, float posY, float posZ, float sX, float sY, float sZ)
{
    modelMatrix.setToIdentity();
    modelMatrix.translate(0, 0, zoom);
    modelMatrix.rotate(trackBall.getRotation());
    modelMatrix.scale(invDiag, invDiag, invDiag);
    modelMatrix.translate(-midPoint);
    modelMatrix.scale(sX, sY, sZ);
    modelMatrix.translate(posX, posY, posZ);

    GLuint locModel = 0;
    GLuint locNormalMatrix = 0;
    GLuint locShininess = 0;
    locModel = glGetUniformLocation(shaderProgram[shaderIndex], "model");
    locNormalMatrix = glGetUniformLocation(shaderProgram[shaderIndex], "normalMatrix");
    locShininess = glGetUniformLocation(shaderProgram[shaderIndex], "shininess");

    glBindVertexArray(vao);
    //glUseProgram(shaderProgram[shaderIndex]);

    glUniformMatrix4fv(locModel, 1, GL_FALSE, modelMatrix.data());
    glUniformMatrix3fv(locNormalMatrix, 1, GL_FALSE, modelMatrix.normalMatrix().data());
    glUniform1f(locShininess, static_cast<GLfloat>(material.shininess));

    if (textureID)
    {
        GLuint locColorTexture = 0;
        locColorTexture = glGetUniformLocation(shaderProgram[shaderIndex], "colorTexture");
        glUniform1i(locColorTexture, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    if (textureLayerID)
    {
        GLuint locColorTextureLayer = 1;
        locColorTextureLayer = glGetUniformLocation(shaderProgram[shaderIndex], "colorTextureLayer");
        glUniform1i(locColorTextureLayer, 1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureLayerID);
    }

    if (textureCubeMapID)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeMapID);
    }

    glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, nullptr);
}

void Model::readOFFFile(QString const &fileName)
{
    std::ifstream stream;
    stream.open(fileName.toUtf8(),std::ifstream::in);
    if (!stream.is_open())
    {
        qWarning("Cannot open file.");
        return;
    }

    std::string line;
    stream >> line;
    stream >> numVertices >> numFaces >> line;

    vertices = std::make_unique<QVector4D[]>(numVertices);
    indices = std::make_unique<unsigned int[]>(numFaces * 3);

    if (numVertices > 0)
    {
        float minLim = std::numeric_limits<float>::lowest();
        float maxLim = std::numeric_limits<float>::max();
        QVector4D max(minLim, minLim, minLim, 1.0);
        QVector4D min(maxLim, maxLim, maxLim, 1.0);
        for (unsigned int i = 0; i < numVertices; ++i)
        {
            float x, y, z;
            stream >> x >> y >> z;
            max.setX(std::max(max.x(), x));
            max.setY(std::max(max.y(), y));
            max.setZ(std::max(max.z(), z));
            min.setX(std::min(min.x(), x));
            min.setY(std::min(min.y(), y));
            min.setZ(std::min(min.z(), z));
            vertices[i] = QVector4D(x, y, z, 1.0);
        }

        midPoint = QVector3D((min + max) * 0.5);
        invDiag = std::sqrt(3.0) / (max - min).length();
    }

    for (unsigned int i = 0; i < numFaces; ++i)
    {
        unsigned int a, b, c;
        stream >> line >> a >> b >> c;
        indices[i * 3 + 0] = a;
        indices[i * 3 + 1] = b;
        indices[i * 3 + 2] = c;
    }
    stream.close();

    createNormals();
    createTexCoords();
    createTangents();
    createShaders();
    createVBOs();
}

void Model::createNormals()
{
    normals = std::make_unique<QVector3D[]>(numVertices);

    for (unsigned int i = 0; i < numFaces; ++i)
    {
        QVector3D a = QVector3D(vertices[indices[i * 3 + 0]]);
        QVector3D b = QVector3D(vertices[indices[i * 3 + 1]]);
        QVector3D c = QVector3D(vertices[indices[i * 3 + 2]]);
        QVector3D faceNormal = QVector3D::crossProduct((b - a), (c - b));

        // Accumulates face normals on the vertices
        normals[indices[i * 3 + 0]] += faceNormal;
        normals[indices[i * 3 + 1]] += faceNormal;
        normals[indices[i * 3 + 2]] += faceNormal;
    }

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        normals[i].normalize();
    }
}

void Model::createTexCoords()
{
    texCoords = std::make_unique<QVector2D[]>(numVertices);

    // Compute minimum and maximum values
    auto minz = std::numeric_limits<float>::max();
    auto maxz = std::numeric_limits<float>::lowest();

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        minz = std::min(vertices[i].z(), minz);
        maxz = std::max(vertices[i].z(), maxz);
    }

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        float s = (std::atan2(vertices[i].y(), vertices[i].x()) + M_PI) / (2 * M_PI);
        float t = 1.0f - (vertices[i].z() - minz) / (maxz - minz);
        texCoords[i] = QVector2D(s, t);
    }
}

void Model::loadTexture(const QImage &image)
{
    if(textureID)
    {
        glDeleteTextures(1, &textureID);
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height (), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Model::createTangents()
{
    tangents = std::make_unique<QVector4D[]>(numVertices);

    std::unique_ptr<QVector3D[]> bitangents;
    bitangents = std::make_unique<QVector3D[]>(numVertices);

    for (unsigned int i = 0; i < numFaces ; ++i)
    {
        unsigned int i1 = indices[i * 3 + 0];
        unsigned int i2 = indices[i * 3 + 1];
        unsigned int i3 = indices[i * 3 + 2];

        QVector3D E = vertices[i1].toVector3D();
        QVector3D F = vertices[i2].toVector3D();
        QVector3D G = vertices[i3].toVector3D();

        QVector2D stE = texCoords[i1];
        QVector2D stF = texCoords[i2];
        QVector2D stG = texCoords[i3];

        QVector3D P = F - E;
        QVector3D Q = G - E;

        QVector2D st1 = stF - stE;
        QVector2D st2 = stG - stE;

        QMatrix2x2 M;
        M(0, 0) =  st2.y();
        M(0, 1) = -st1.y();
        M(1, 0) = -st2.x();
        M(1, 1) =  st1.x();
        M *= (1.0f / (st1.x() * st2.y() - st2.x() * st1.y()));

        QVector4D T = QVector4D (M(0, 0) * P.x() + M(0, 1) * Q.x(),
                                 M(0, 0) * P.y() + M(0, 1) * Q.y(),
                                 M(0, 0) * P.z() + M(0, 1) * Q.z(), 0.0);

        QVector3D B = QVector3D (M(1, 0) * P.x() + M(1, 1) * Q.x(),
                                 M(1, 0) * P.y() + M(1, 1) * Q.y(),
                                 M(1, 0) * P.z() + M(1, 1) * Q.z());
        tangents[i1] += T;
        tangents[i2] += T;
        tangents[i3] += T;

        bitangents[i1] += B;
        bitangents[i2] += B;
        bitangents[i3] += B;
    }

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        const QVector3D &n = normals[i];
        const QVector4D &t = tangents[i];
        tangents[i] = (t - n * QVector3D::dotProduct(n, t.toVector3D())).normalized();
        QVector3D b = QVector3D::crossProduct(n, t.toVector3D());
        float hand = QVector3D::dotProduct(b, bitangents[i]);
        tangents[i].setW((hand < 0.0f) ? -1.0f : 1.0f);
    }
}
