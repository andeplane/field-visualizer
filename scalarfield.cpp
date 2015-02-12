#include "scalarfield.h"
#include <QColor>
#include <QRgb>
#include <iostream>
#include <QFile>

using std::cout; using std::endl;
using std::vector;

ScalarField::ScalarField() :
    m_numPointsX(0),
    m_numPointsY(0),
    m_indicesDirty(true),
    m_funcs(0),
    m_program(0)
{
    generateVBOs();
}

ScalarField::~ScalarField()
{
    if(m_funcs) delete m_funcs;
    if(m_program) delete m_program;
}

void ScalarField::generateVBOs()
{
    ensureInitialized();
    m_funcs->glGenBuffers(2, m_vboIds);
}

void ScalarField::ensureInitialized()
{
    if(!m_funcs) m_funcs = new QOpenGLFunctions(QOpenGLContext::currentContext());
}

void ScalarField::update(ScalarFieldData &data)
{
    ensureInitialized();
    if(data.numPointsX == 0) return;
    resize(data.numPointsX, data.numPointsY);

    // Transfer vertex data to VBO 0
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    m_funcs->glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(ScalarFieldVertex), &m_vertices[0], GL_STATIC_DRAW);
    if(m_indicesDirty) {
        m_funcs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
        m_funcs->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
        m_indicesDirty = false;
    }
}

void ScalarField::createShaderProgram() {
    if (!m_program) {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader.frag");



        m_program->link();
    }
}

void ScalarField::render(const QMatrix4x4 &modelViewProjectionMatrix, QVector3D cameraPosition, float time, float scaling)
{
    if(m_vertices.size() == 0) return;
    ensureInitialized();
    createShaderProgram();
    float lightFalloffDistance = 500.0;
    m_program->bind();

    m_program->setUniformValue("modelViewProjectionMatrix", modelViewProjectionMatrix);
    m_program->setUniformValue("cameraPosition", cameraPosition);
    m_program->setUniformValue("time", time);
    m_program->setUniformValue("lightFalloffDistance", lightFalloffDistance);
    m_program->setUniformValue("scaling", scaling);

    // Tell OpenGL which VBOs to use
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    m_funcs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray(vertexLocation);
    m_funcs->glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(ScalarFieldVertex), (const void *)offset);
    glEnable(GL_DEPTH_TEST);
    m_funcs->glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    // m_funcs->glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, 0);
    m_program->disableAttributeArray(vertexLocation);

    m_program->release();
}

void ScalarField::resize(unsigned int numPointsX, unsigned int numPointsY)
{
    if(m_numPointsX == numPointsX && m_numPointsY == numPointsY) return; // Not changed
    m_numPointsX = numPointsX;
    m_numPointsY = numPointsY;
    unsigned int numPoints = numPointsX*numPointsY;
    int numTriangles = 2*(numPointsX-1)*(numPointsY-1);
    int numIndices = 3*numTriangles;
    m_vertices.resize(numPoints);
    m_indices.clear();
    m_indices.reserve(numIndices);
    for(int i=0; i<int(numPointsX); i++) {
        for(int j=0; j<int(numPointsY); j++) {
            float x = 500*(float(i)/(numPointsX-1) - 0.5);
            float y = 500*(float(j)/(numPointsY-1) - 0.5);
            float z = 0.0;

            unsigned int idx = index(i, j);
            m_vertices[idx].position = QVector3D(x,y,z);

            if(i < int(numPointsX)-1 && j < int(numPointsY)-1) {
                unsigned int index1 = index(i+1,j);
                unsigned int index2 = index(i,j+1);
                unsigned int index3 = index(i,j);
                m_indices.push_back(index1);
                m_indices.push_back(index2);
                m_indices.push_back(index3);

                index1 = index(i+1,j+1);
                index2 = index(i,j+1);
                index3 = index(i+1,j);
                m_indices.push_back(index1);
                m_indices.push_back(index2);
                m_indices.push_back(index3);
            }
        }
    }
    m_indicesDirty = true;
}
