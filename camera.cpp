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

    // m_modelViewMatrix.lookAt(m_position, m_position + m_forwardVector, m_upVector);
    m_modelViewMatrix.rotate(90, 1, 0, 0);

    //m_modelViewMatrix.rotate(m_rotation);
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

    long currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    long timeDifference = currentTime-m_lastTime;
    float timeDifferenceSeconds = float(timeDifference)/1000.0;

    m_lastTime = currentTime;

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
    if(m_movingForward) {
        m_forwardSpeed = speed*timeDifferenceSeconds;
    } else if(m_movingBackward) {
        m_forwardSpeed = -speed*timeDifferenceSeconds;
    } else {
        m_forwardSpeed = 0;
    }

    if(m_movingRight) {
        m_rightSpeed = speed*timeDifferenceSeconds;
    } else if(m_movingLeft) {
        m_rightSpeed = -speed*timeDifferenceSeconds;
    } else {
        m_rightSpeed = 0;
    }

    translation += forwardVector*m_forwardSpeed;
    translation += rightVector*m_rightSpeed;
    setPosition(m_position + translation);
}

void Camera::mouseMoved(float deltaX, float deltaY)
{
    deltaX *= m_mouseSensitivity;
    deltaY *= m_mouseSensitivity;
    if(m_tilt - deltaY > -90 && m_tilt - deltaY < 90) {
        setTilt(m_tilt - deltaY);
    }

    setPan(m_pan - deltaX);
    return;

//    qDebug() << endl << "Starting rotation";
//    qDebug() << "(dx, dy) = (" << deltaX << ", " << deltaY << ")";

//    deltaX *= -m_mouseSensitivity*DEGTORAD;
//    deltaY *= -m_mouseSensitivity*DEGTORAD;
//    deltaX = 0.0;
//    deltaY = -0.01;

//    QVector3D rightVector = QVector3D::crossProduct(m_forwardVector, m_upVector);
//    QQuaternion rightAxis = QQuaternion(0.0, rightVector);
//    QQuaternion upAxis = QQuaternion(0.0, m_upVector);
//    QQuaternion forwardAxis = QQuaternion(0.0, m_forwardVector);
//    qDebug() << "Forward: " << m_forwardVector;
//    qDebug() << "Up: " << m_upVector;
//    qDebug() << "Right: " << rightAxis.vector();

//    float sinThetaX = sin(deltaX*0.5);
//    qDebug() << "sin(thetaX): " << sinThetaX;
//    QQuaternion rotation(cos(deltaX*0.5), sinThetaX*m_upVector);
//    qDebug() << "Before: : " << m_rotation;
//    m_rotation = (rotation*m_rotation)*rotation.conjugate();
//    qDebug() << "After: m_rotation: " << m_rotation;

//    forwardAxis = rotation.conjugate()*(forwardAxis*rotation);
//    rightAxis = rotation.conjugate()*(rightAxis*rotation);

//    //    qDebug() << "After x-rotation:";
//    //    qDebug() << "Forward: " << forwardAxis.vector();
//    //    qDebug() << "Up: " << upAxis.vector();

//    float sinThetaY = sin(deltaY*0.5);
//    qDebug() << "sin(thetaY): " << sinThetaY;
//    rotation = QQuaternion(cos(deltaY*0.5), sinThetaY*rightAxis.vector());
//    m_rotation = (rotation*m_rotation)*rotation.conjugate();

//    forwardAxis = rotation.conjugate()*(forwardAxis*rotation);
//    upAxis = rotation.conjugate()*(upAxis*rotation);

//    qDebug() << "After y-rotation:";
//    qDebug() << "Forward: " << forwardAxis.vector();
//    qDebug() << "Up: " << upAxis.vector();

//    m_upVector = upAxis.vector();
//    m_forwardVector = forwardAxis.vector();
//    m_forwardQuat = forwardAxis;
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
    m_movingForward(false),
    m_movingBackward(false),
    m_movingLeft(false),
    m_movingRight(false),
    m_forwardSpeed(0),
    m_rightSpeed(0),
    m_lastTime(0),
    m_mouseSensitivity(0.03),
    m_forwardVector(QVector3D(1.0, 0.0, 0.0)),
    m_upVector(QVector3D(0.0, 0.0, 1.0)),
    m_rotation(QQuaternion(1.0, 0.0, 0.0, 0.0))
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerTicked()));
    m_timer.setInterval(16);
    m_timer.start(16);
}

Camera::~Camera()
{

}
