#include "renderer.h"
#include "controller.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QGLFormat>
#include <QOpenGLContext>
#include <iostream>
#include <cmath>
#include <QOpenGLFramebufferObjectFormat>

using namespace std;
namespace CompPhys {

void Renderer::synchronize(QQuickFramebufferObject* item)
{
    m_syncCount++;
    Controller *controller = (Controller*)item; // TODO: Use correct casting method
    if(!controller) {
        return;
    }

    resetProjection();
    setModelViewMatrices(controller->camera().position, controller->camera().tilt, controller->camera().pan, controller->camera().roll);

    if(controller->simulatorOutputDirty()) {
        controller->m_simulatorOutputMutex.lock();
        // Read output
        m_renderScalarField = controller->renderScalarField();
        m_data.scalarFieldData = controller->m_visualData.scalarFieldData;

        m_scalarField.update(m_data.scalarFieldData);

        controller->setSimulatorOutputDirty(false);
        controller->m_simulatorOutputMutex.unlock();
        m_dirtyCount++;
    }
}

void Renderer::render()
{
    m_renderCount++;
    glDepthMask(true);

    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    // Render data
    QMatrix4x4 modelViewProjectionMatrix = m_projectionMatrix * m_modelViewMatrix;

    if(m_renderScalarField) m_scalarField.render(modelViewProjectionMatrix, m_time);
    m_time+=1e-2;
}

void Renderer::resetProjection()
{
    // Calculate aspect ratio
    qreal aspect = qreal(m_viewportSize.width()) / qreal(m_viewportSize.height() ? m_viewportSize.height() : 1);
    // Set near plane to 3.0, far plane to 7.0, field of view 65 degrees
    const qreal zNear = 0.1, zFar = 2000.0, fov = 65.0;

    // Reset projection
    m_projectionMatrix.setToIdentity();

    // Set perspective projection
    m_projectionMatrix.perspective(fov, aspect, zNear, zFar);
}
QSize Renderer::viewportSize() const
{
    return m_viewportSize;
}

void Renderer::setViewportSize(const QSize &viewportSize)
{
    m_viewportSize = viewportSize;
}


void Renderer::setModelViewMatrices(QVector3D position, double tilt, double pan, double roll)
{
    m_modelViewMatrix.setToIdentity();
    m_modelViewMatrix.translate(position);
    m_modelViewMatrix.rotate(90, 1, 0, 0);
    m_modelViewMatrix.rotate(tilt, 1, 0, 0);
    m_modelViewMatrix.rotate(pan, 0, 0, 1);
    m_modelViewMatrix.rotate(roll, 0, 1, 0);
}

Renderer::Renderer() :
    m_skipNextFrame(false),
    m_renderPoints(true),
    m_renderScalarField(true),
    m_time(0),
    m_syncCount(0),
    m_renderCount(0),
    m_dirtyCount(0)
{
    m_data.scalarFieldData.numPointsX = 0;
    m_data.scalarFieldData.numPointsY = 0;
}

Renderer::~Renderer()
{

}

QOpenGLFramebufferObject *Renderer::createFramebufferObject(const QSize &size) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    m_viewportSize = size;
    return new QOpenGLFramebufferObject(size, format);
}
}
