#pragma once

#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <QtGui/QQuaternion>
#include <QtGui/QVector3D>

namespace viz {

class CDataProvider final
{
public: // types
    using DataType = QVector3D;
    using ContainerType = std::vector<DataType>;
    using iterator = std::vector<ContainerType>::iterator;

public: // methods
    explicit CDataProvider();
    ~CDataProvider();

    iterator begin();
    iterator end();

    void lock();
    void unlock();

    void toggleRecording();
    void clear();
    void mark();

private: // methods
    void inputThread();

private: // fields
    std::mutex m_mutex;
    std::thread m_input_thread;
    std::vector<ContainerType> m_positions;
    std::vector<QQuaternion> m_rotations;
    std::vector<std::size_t> m_marks;
    std::atomic_bool m_stopped;
    bool m_recording = false;
};

} // namespace viz
