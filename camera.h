#ifndef CAMERA_H
#define CAMERA_H
#define DEGTORAD M_PI/180.0

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QSize>

class Camera : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(float tilt READ tilt WRITE setTilt NOTIFY tiltChanged)
    Q_PROPERTY(float pan READ pan WRITE setPan NOTIFY panChanged)
    Q_PROPERTY(float roll READ roll WRITE setRoll NOTIFY rollChanged)
    Q_PROPERTY(QSize viewportSize READ viewportSize WRITE setViewportSize NOTIFY viewportSizeChanged)
    Q_PROPERTY(QVector3D forwardVector READ forwardVector)
    Q_PROPERTY(QVector3D rightVector READ rightVector)
    Q_PROPERTY(QVector3D upVector READ upVector)
    Q_PROPERTY(bool fixedPosition READ fixedPosition WRITE setFixedPosition NOTIFY fixedPositionChanged)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
    Q_PROPERTY(float fieldOfView READ fieldOfView WRITE setFieldOfView NOTIFY fieldOfViewChanged)

private:
    QVector3D m_position;

    float m_tilt;

    float m_pan;

    float m_roll;

    QMatrix4x4 m_projectionMatrix;

    QMatrix4x4 m_modelViewMatrix;

    bool m_fixedPosition;

    QSize m_viewportSize;

    float m_fieldOfView;

    float m_farPlane;

    float m_nearPlane;

public:
    explicit Camera(QObject *parent = 0);
    ~Camera();
    QVector3D position() const
    {
        return m_position;
    }
    float tilt() const
    {
        return m_tilt;
    }

    float pan() const
    {
        return m_pan;
    }

    float roll() const
    {
        return m_roll;
    }

    QVector3D forwardVector()
    {
        float x = cos(m_pan*DEGTORAD)*cos(m_tilt*DEGTORAD);
        float y = sin(m_pan*DEGTORAD)*cos(m_tilt*DEGTORAD);
        float z = sin(m_tilt*DEGTORAD);
        return QVector3D(x,y,z);
    }


    QVector3D upVector()
    {
        QVector3D forwardVector = this->forwardVector();
        float x = -forwardVector.z()*forwardVector.x()/sqrt(forwardVector.x()*forwardVector.x() + forwardVector.y()*forwardVector.y());
        float y = -forwardVector.z()*forwardVector.y()/sqrt(forwardVector.x()*forwardVector.x() + forwardVector.y()*forwardVector.y());
        float z = sqrt(forwardVector.x()*forwardVector.x() + forwardVector.y()*forwardVector.y());
        return QVector3D(x,y,z);
    }

    QVector3D rightVector()
    {
        QVector3D forwardVector = this->forwardVector();
        QVector3D upVector = this->upVector();
        return QVector3D::crossProduct(forwardVector, upVector);
    }

    QMatrix4x4 projectionMatrix();
    void setProjectionMatrix(const QMatrix4x4 &projectionMatrix);

    QMatrix4x4 modelViewMatrix();
    void setModelViewMatrix(const QMatrix4x4 &modelViewMatrix);

    bool fixedPosition() const
    {
        return m_fixedPosition;
    }

    QSize viewportSize() const
    {
        return m_viewportSize;
    }

    float fieldOfView() const
    {
        return m_fieldOfView;
    }

    float farPlane() const
    {
        return m_farPlane;
    }

    float nearPlane() const
    {
        return m_nearPlane;
    }

public slots:
    void setPosition(QVector3D arg)
    {
        if (m_position == arg)
            return;

        m_position = arg;
        emit positionChanged(arg);
    }
    void setTilt(float arg)
    {
        if (m_tilt == arg)
            return;

        m_tilt = arg;
        emit tiltChanged(arg);
    }

    void setPan(float arg)
    {
        if (m_pan == arg)
            return;

        m_pan = arg;
        emit panChanged(arg);
    }

    void setRoll(float arg)
    {
        if (m_roll == arg)
            return;

        m_roll = arg;
        emit rollChanged(arg);
    }

    void setFixedPosition(bool arg)
    {
        if (m_fixedPosition == arg)
            return;

        m_fixedPosition = arg;
        emit fixedPositionChanged(arg);
    }

    void setViewportSize(QSize arg)
    {
        if (m_viewportSize == arg)
            return;

        m_viewportSize = arg;
        emit viewportSizeChanged(arg);
    }

    void setFieldOfView(float arg)
    {
        if (m_fieldOfView == arg)
            return;

        m_fieldOfView = arg;
        emit fieldOfViewChanged(arg);
    }

    void setFarPlane(float arg)
    {
        if (m_farPlane == arg)
            return;

        m_farPlane = arg;
        emit farPlaneChanged(arg);
    }

    void setNearPlane(float arg)
    {
        if (m_nearPlane == arg)
            return;

        m_nearPlane = arg;
        emit nearPlaneChanged(arg);
    }

signals:
    void positionChanged(QVector3D arg);
    void tiltChanged(float arg);
    void panChanged(float arg);
    void rollChanged(float arg);
    void fixedPositionChanged(bool arg);
    void viewportSizeChanged(QSize arg);
    void fieldOfViewChanged(float arg);
    void farPlaneChanged(float arg);
    void nearPlaneChanged(float arg);
};

#endif // CAMERA_H
