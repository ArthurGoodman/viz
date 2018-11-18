#pragma once

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
    using iterator = ContainerType::iterator;

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
    ContainerType m_data;
};

} // namespace viz
