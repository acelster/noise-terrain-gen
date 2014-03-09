#include "transform3d.hpp"

namespace nmgui {

Transform3D::Transform3D(QObject *parent) :
    QObject(parent),
    m_orientation(0, 1, 0, 0),
    m_position(0,0,0)
{
}

QVector3D Transform3D::position() const
{
    return m_position;
}

void Transform3D::setPosition(const QVector3D &position)
{
    m_position = position;
    positionChanged();
}

QQuaternion Transform3D::orientation() const
{
    return m_orientation;
}

void Transform3D::setOrientation(const QQuaternion &orientation)
{
    m_orientation = orientation;
    orientationChanged();
}

void Transform3D::moveForward(float distance)
{
    m_position += m_orientation.rotatedVector(QVector3D(0,0,-1)) * distance;
    positionChanged();
}

void Transform3D::moveRight(float distance)
{
    m_position += m_orientation.rotatedVector(QVector3D(1,0,0)) * distance;
    positionChanged();
}

void Transform3D::yaw(float angle)
{
    m_orientation *= QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), angle);
}

/* Build a unit quaternion representing the rotation
 * from u to v. The input vectors need not be normalised. */
QQuaternion fromTwoVectors(QVector3D u, QVector3D v)
{
    float norm_u_norm_v = sqrt(QVector3D::dotProduct(u, u) * QVector3D::dotProduct(v, v));
    float real_part = norm_u_norm_v + QVector3D::dotProduct(u, v);
    QVector3D w;
    if (real_part < 1.e-6f * norm_u_norm_v)
    {
        /* If u and v are exactly opposite, rotate 180 degrees
         * around an arbitrary orthogonal axis. Axis normalisation
         * can happen later, when we normalise the quaternion. */
        real_part = 0.0f;
        w = fabs(u.x()) > fabs(u.z()) ? QVector3D(-u.y(), u.x(), 0.f)
                                      : QVector3D(0.f,   -u.z(), u.y());
    } else {
        /* Otherwise, build quaternion the standard way. */
        w = QVector3D::crossProduct(u, v);
    }
    return QQuaternion(real_part, w).normalized();
}

void Transform3D::lookAt(QVector3D position)
{
    QVector3D direction = (position - m_position).normalized();
    //TODO, preserve up
    setOrientation(fromTwoVectors(QVector3D(0,0,-1), direction));
}

} // namespace nmgui