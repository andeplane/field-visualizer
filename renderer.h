#pragma once

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QElapsedTimer>
#include <QMatrix4x4>
#include <QQuickFramebufferObject>
#include <QThread>
#include <QMutex>
#include "scalarfield.h"

namespace CompPhys {
struct VisualData {
    ScalarFieldData scalarFieldData;
};

class Renderer : public QQuickFramebufferObject::Renderer
{
public:
    Renderer();
    ~Renderer();

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
    void synchronize(QQuickFramebufferObject *item);
    void render();

    void setModelViewMatrices(QVector3D position, double tilt, double pan, double roll);
    void resetProjection();
    QSize viewportSize() const;
    void setViewportSize(const QSize &viewportSize);

private:
    bool m_skipNextFrame;
    bool m_renderPoints;
    bool m_renderScalarField;
    float m_time;
    QVector3D m_cameraPosition;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrix;
    QSize m_viewportSize;
    ScalarField m_scalarField;
    VisualData m_data;

    int m_syncCount;
    int m_renderCount;
    int m_dirtyCount;
};
}
