#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

namespace {

class View final : public QWidget
{
    Q_OBJECT

public: // methods
    explicit View()
        : m_input_thread{std::bind(&View::inputThread, this)}
    {
    }

    ~View()
    {
        if (m_input_thread.joinable())
        {
            m_input_thread.detach();
        }
    }

protected: // methods
    void keyPressEvent(QKeyEvent *e) override
    {
        switch (e->key())
        {
        case Qt::Key_Q:
        case Qt::Key_Escape:
            if (isFullScreen())
            {
                showNormal();
            }
            else
            {
                close();
            }
            break;

        case Qt::Key_F:
        case Qt::Key_F11:
            if (isFullScreen())
            {
                showNormal();
            }
            else
            {
                showFullScreen();
            }
            break;
        }
    }

    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.fillRect(rect(), Qt::lightGray);

        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(QPen{Qt::red});

        {
            std::unique_lock<std::mutex> guard{m_mutex};

            if (!m_points.empty())
            {
                QPointF prev_point = m_points.front();
                for (std::size_t i = 1; i < m_points.size(); i++)
                {
                    static constexpr double c_scale = 100;
                    QPointF offset = rect().center();

                    p.drawLine(
                        c_scale * prev_point + offset,
                        c_scale * m_points[i] + offset);

                    prev_point = m_points[i];
                }
            }
        }

        QTimer::singleShot(16, this, SLOT(update()));
    }

private: // methods
    void inputThread()
    {
        while (std::cin)
        {
            QPointF point;
            std::cin >> point.rx() >> point.ry();

            if (std::cin)
            {
                std::unique_lock<std::mutex> guard{m_mutex};
                m_points.emplace_back(point);
            }
        }
    }

private: // fields
    std::mutex m_mutex;
    std::thread m_input_thread;
    std::vector<QPointF> m_points;
};

} // anonymous namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    View view;
    view.show();

    return app.exec();
}

#include "viz.moc"
