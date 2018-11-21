#pragma once

#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
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

private: // methods
    void inputThread();

private: // fields
    std::mutex m_mutex;
    std::thread m_input_thread;
    std::vector<ContainerType> m_data;
    std::atomic_bool m_stopped;
};

} // namespace viz
