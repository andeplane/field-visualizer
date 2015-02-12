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

    m_projectionMatrix = controller->camera()->projectionMatrix();
    m_modelViewMatrix = controller->camera()->modelViewMatrix();
    m_cameraPosition = controller->camera()->position();

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

    if(m_renderScalarField) m_scalarField.render(modelViewProjectionMatrix, m_cameraPosition, m_time, m_scaling);
    m_time+=1e-2;
}

QSize Renderer::viewportSize() const
{
    return m_viewportSize;
}

void Renderer::setViewportSize(const QSize &viewportSize)
{
    m_viewportSize = viewportSize;
}

Renderer::Renderer() :
    m_skipNextFrame(false),
    m_renderPoints(true),
    m_renderScalarField(true),
    m_time(0),
    m_scaling(0.01),
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
