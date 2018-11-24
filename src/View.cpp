#include "View.hpp"
#include <cmath>
#include <QtGui/QKeyEvent>
#include "Renderer.hpp"

namespace viz {

CView::CView(CRenderer &renderer)
    : m_renderer{renderer}
{
    setSurfaceType(QWindow::OpenGLSurface);

    setFormat(m_renderer.format());

    m_renderer.setView(this);

    create();
}

void CView::exposeEvent(QExposeEvent *)
{
    static bool s_first_expose = true;
    if (isExposed() && s_first_expose)
    {
        s_first_expose = false;
        m_renderer.render();
    }
}

void CView::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_F:
    case Qt::Key_F11:
        if (visibility() == QWindow::FullScreen)
        {
            showNormal();
        }
        else
        {
            showFullScreen();
        }
        break;

    case Qt::Key_Escape:
        if (visibility() == QWindow::FullScreen)
        {
            showNormal();
        }
        else
        {
            close();
        }
        break;

    case Qt::Key_Q:
        close();
        break;

    case Qt::Key_C:
        m_renderer.clear();
        break;

    case Qt::Key_Return:
        m_renderer.toggleRecording();
        break;
    }
}

void CView::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        m_prev_pos = e->pos();
    }
}

void CView::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        int delta_x = e->x() - m_prev_pos.x();
        int delta_y = e->y() - m_prev_pos.y();

        QQuaternion rot_conj = m_renderer.getRotation().conjugate();

        QVector3D view_dir =
            (m_renderer.getCenter() - m_renderer.getEye()).normalized();
        QVector3D up = m_renderer.getUp().normalized();

        QVector3D horizontal_axis =
            rot_conj * QVector3D::crossProduct(view_dir, up);
        QVector3D vertical_axis = rot_conj * m_renderer.getUp();

        if (e->modifiers() & Qt::ShiftModifier)
        {
            QMatrix4x4 view_projection = m_renderer.getViewProjection();

            float sensitivity = 2.0f / height() / (view_projection * up).y();

            m_renderer.translate(sensitivity * delta_x * horizontal_axis);
            m_renderer.translate(sensitivity * -delta_y * vertical_axis);
        }
        else
        {
            float sensitivity = 180.0f / height();

            QQuaternion q_x = QQuaternion::fromAxisAndAngle(
                horizontal_axis, sensitivity * delta_y);
            QQuaternion q_y = QQuaternion::fromAxisAndAngle(
                vertical_axis, sensitivity * delta_x);

            m_renderer.rotate(q_x * q_y);
        }

        m_prev_pos = e->pos();
    }
}

void CView::wheelEvent(QWheelEvent *e)
{
    static constexpr float c_scale_factor = 1.1f;

    if (e->delta() > 0)
    {
        m_renderer.scale(c_scale_factor);
    }
    else
    {
        m_renderer.scale(1.0f / c_scale_factor);
    }
}

} // namespace viz
