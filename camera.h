#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QVector3D>

class Camera : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(float tilt READ tilt WRITE setTilt NOTIFY tiltChanged)
    Q_PROPERTY(float pan READ pan WRITE setPan NOTIFY panChanged)
    Q_PROPERTY(float roll READ roll WRITE setRoll NOTIFY rollChanged)
    Q_PROPERTY(QVector3D forwardVector READ forwardVector)
    Q_PROPERTY(QVector3D rightVector READ rightVector)
    Q_PROPERTY(QVector3D upVector READ upVector)

private:
    QVector3D m_position;

    float m_tilt;

    float m_pan;

    float m_roll;

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
        float degToRad = M_PI/180.0;
        float x = cos(m_pan*degToRad)*cos(m_tilt*degToRad);
        float y = sin(m_pan*degToRad)*cos(m_tilt*degToRad);
        float z = sin(m_tilt*degToRad);
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

signals:
    void positionChanged(QVector3D arg);
    void tiltChanged(float arg);
    void panChanged(float arg);
    void rollChanged(float arg);
};

#endif // CAMERA_H
