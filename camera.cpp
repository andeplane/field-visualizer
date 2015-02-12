#include "camera.h"


QMatrix4x4 Camera::projectionMatrix()
{
    // Calculate aspect ratio

    qreal aspect = qreal(m_viewportSize.width()) / qreal(m_viewportSize.height() ? m_viewportSize.height() : 1);

    // Reset projection
    m_projectionMatrix.setToIdentity();

    // Set perspective projection
    m_projectionMatrix.perspective(m_fieldOfView, aspect, m_nearPlane, m_farPlane);

    return m_projectionMatrix;
}

void Camera::setProjectionMatrix(const QMatrix4x4 &projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;
}

QMatrix4x4 Camera::modelViewMatrix()
{
    m_modelViewMatrix.setToIdentity();
    if(!m_fixedPosition) {
        m_modelViewMatrix.translate(m_position);
    }

    m_modelViewMatrix.rotate(90, 1, 0, 0);
    m_modelViewMatrix.rotate(m_tilt, 1, 0, 0);
    m_modelViewMatrix.rotate(m_pan, 0, 0, 1);

    return m_modelViewMatrix;
}

void Camera::setModelViewMatrix(const QMatrix4x4 &modelViewMatrix)
{
    m_modelViewMatrix = modelViewMatrix;
}
Camera::Camera(QObject *parent) :
    QObject(parent),
    m_position(QVector3D(0,0,5)),
    m_tilt(0),
    m_pan(0),
    m_roll(0),
    m_fixedPosition(false),
    m_fieldOfView(65.0),
    m_farPlane(2000.0),
    m_nearPlane(0.1)
{


}

Camera::~Camera()
{

}
