#include "camera.h"
#include <QDateTime>

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

void Camera::timerTicked()
{

    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    qint64 timeDifference = currentTime-m_state.lastTime;
    float timeDifferenceSeconds = float(timeDifference)/1000.0;

    m_state.lastTime = currentTime;

    if(timeDifference > 1000) {
        return;
    }

    QVector3D forwardVector = this->forwardVector();
    QVector3D rightVector = this->rightVector();
    QVector3D translation;
    float speed = m_moveSpeed;
    if(m_hyperSpeed) {
        speed *= m_hyperSpeedFactor;
    }

    // Decide what to do based on velocity
    if(m_state.forward) {
        m_state.forwardSpeed = speed*timeDifferenceSeconds;
    } else if(m_state.backward) {
        m_state.forwardSpeed = -speed*timeDifferenceSeconds;
    } else {
        m_state.forwardSpeed = 0;
    }

    if(m_state.right) {
        m_state.rightSpeed = speed*timeDifferenceSeconds;
    } else if(m_state.left) {
        m_state.rightSpeed = -speed*timeDifferenceSeconds;
    } else {
        m_state.rightSpeed = 0;
    }

    translation += forwardVector*m_state.forwardSpeed;
    translation += rightVector*m_state.rightSpeed;
    setPosition(m_position + translation);
}

void Camera::mouseMoved(float deltaX, float deltaY)
{
    deltaX *= m_mouseSensitivity;
    deltaY *= m_mouseSensitivity;

    if(m_tilt - deltaY > -90 && m_tilt - deltaY < 90) {
        m_tilt -= deltaY;
    }

    m_pan -= deltaX;
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
    m_nearPlane(0.1),
    m_hyperSpeed(false),
    m_moveSpeed(3.0),
    m_hyperSpeedFactor(4.0),
    m_mouseSensitivity(0.03)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerTicked()));
    m_timer.setInterval(16);
    m_timer.start(16);

}

Camera::~Camera()
{

}
