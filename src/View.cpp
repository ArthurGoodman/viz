#include "View.hpp"
#include <cmath>
#include <QtGui/QKeyEvent>
#include "Renderer.hpp"

#include <iostream>

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

        QVector3D x_axis = rot_conj * QVector3D{1.0f, 0.0f, 0.0f};
        QVector3D y_axis = rot_conj * QVector3D{0.0f, 1.0f, 0.0f};

        if (e->modifiers() & Qt::ShiftModifier)
        {
            ///@todo Simplify these computations

            QMatrix4x4 mvp;
            mvp.perspective(45, (float)width() / height(), 1e-5f, 100);
            mvp.lookAt(
                QVector3D{0, 0, 3}, QVector3D{0, 0, 0}, QVector3D{0, 1, 0});
            mvp.scale(m_renderer.getScale());

            float sensitivity =
                1.0f /
                (0.5f * height() * (mvp * QVector3D{0.0f, 1.0f, 0.0f}).y());

            m_renderer.translate(sensitivity * delta_x * x_axis);
            m_renderer.translate(sensitivity * -delta_y * y_axis);
        }
        else
        {
            float sensitivity = M_PI / height();

            float x_angle = sensitivity * delta_y;
            float y_angle = sensitivity * delta_x;

            QQuaternion q_x{std::cos(x_angle / 2.0f),
                            std::sin(x_angle / 2.0f) * x_axis};
            QQuaternion q_y{std::cos(y_angle / 2.0f),
                            std::sin(y_angle / 2.0f) * y_axis};

            m_renderer.rotate(q_x * q_y);
        }

        m_prev_pos = e->pos();
    }
}

void CView::wheelEvent(QWheelEvent *e)
{
    static constexpr float c_factor = 1.1f;

    if (e->delta() > 0)
    {
        m_renderer.scale(c_factor);
    }
    else
    {
        m_renderer.scale(1.0f / c_factor);
    }
}

} // namespace viz
