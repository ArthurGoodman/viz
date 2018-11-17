#include "View.hpp"
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
    if (isExposed())
    {
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

    case Qt::Key_Q:
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
    }
}

} // namespace viz
