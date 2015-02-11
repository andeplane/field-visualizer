#pragma once
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <vector>

struct ScalarFieldData
{
    int numPointsX;
    int numPointsY;
};

struct ScalarFieldVertex
{
    QVector3D position;
};

class ScalarField
{
public:
    ScalarField();
    ~ScalarField();
    void update(ScalarFieldData &data);
    void render(const QMatrix4x4 &modelViewProjectionMatrix, QVector3D cameraPosition, float time, float scaling);
    void resize(unsigned int numPointsX, unsigned int numPointsY);
private:
    unsigned int m_numPointsX;
    unsigned int m_numPointsY;
    bool m_indicesDirty;
    GLuint m_vboIds[2];

    std::vector<ScalarFieldVertex> m_vertices;
    std::vector<unsigned int>    m_indices;
    QOpenGLFunctions *m_funcs;
    QOpenGLShaderProgram *m_program;

    void createShaderProgram();
    void generateVBOs();
    void ensureInitialized();
    unsigned int index(int i, int j) { return i + j*m_numPointsY; }
};
