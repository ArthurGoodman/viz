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

private: // fields
    CRenderer &m_renderer;
};

} // namespace viz
