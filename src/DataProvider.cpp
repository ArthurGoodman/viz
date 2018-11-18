#include "DataProvider.hpp"
#include <functional>
#include <iostream>
#include <thread>

namespace viz {

CDataProvider::CDataProvider()
    : m_input_thread{std::bind(&CDataProvider::inputThread, this)}
{
}

CDataProvider::~CDataProvider()
{
    if (m_input_thread.joinable())
    {
        m_input_thread.detach();
    }
}

CDataProvider::iterator CDataProvider::begin()
{
    return m_data.begin();
}

CDataProvider::iterator CDataProvider::end()
{
    return m_data.end();
}

void CDataProvider::lock()
{
    m_mutex.lock();
}

void CDataProvider::unlock()
{
    m_mutex.unlock();
}

void CDataProvider::inputThread()
{
    while (std::cin)
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        std::cin >> x >> y >> z;

        if (std::cin)
        {
            std::unique_lock<std::mutex> guard{m_mutex};
            m_data.emplace_back(x, y, z);
        }
    }
}

} // namespace viz
