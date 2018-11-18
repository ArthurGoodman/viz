#pragma once

#include <QtGui/QWindow>

namespace viz {

class CRenderer;

class CView final : public QWindow
{
public: // methods
    explicit CView(CRenderer &renderer);

protected: // methods
    void exposeEvent(QExposeEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;

private: // fields
    CRenderer &m_renderer;

    QPoint m_prev_pos;
};

} // namespace viz
