#include "DataProvider.hpp"
#include <functional>
#include <iostream>
#include <sstream>
#include <thread>

namespace viz {

CDataProvider::CDataProvider()
    : m_input_thread{std::bind(&CDataProvider::inputThread, this)}
{
    m_data.push_back(ContainerType());
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
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line == "+")
        {
            std::unique_lock<std::mutex> guard{m_mutex};
            m_data.push_back(ContainerType());
        }

        std::stringstream ss{line};

        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        ss >> x >> y >> z;

        if (ss)
        {
            std::unique_lock<std::mutex> guard{m_mutex};
            m_data.back().emplace_back(x, y, z);
        }
    }
}

} // namespace viz
