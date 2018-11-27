#include "DataProvider.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <thread>
#include <QtCore/QTime>

namespace viz {

CDataProvider::CDataProvider()
    : m_input_thread{std::bind(&CDataProvider::inputThread, this)}
    , m_stopped{false}
{
}

CDataProvider::~CDataProvider()
{
    m_stopped.store(true);
    m_input_thread.join();
}

CDataProvider::iterator CDataProvider::begin()
{
    return m_positions.begin();
}

CDataProvider::iterator CDataProvider::end()
{
    return m_positions.end();
}

void CDataProvider::lock()
{
    m_mutex.lock();
}

void CDataProvider::unlock()
{
    m_mutex.unlock();
}

void CDataProvider::toggleRecording()
{
    if (m_recording)
    {
        std::unique_lock<std::mutex> guard{m_mutex};

        std::string dump_name = "./logs/" +
                                QDateTime::currentDateTime()
                                    .toString(Qt::DateFormat::ISODate)
                                    .toStdString() +
                                ".log";

        std::ofstream file(dump_name);

        std::size_t i = 0;
        for (; i < m_positions.size() && i < m_rotations.size(); i++)
        {
            const auto &pos = m_positions.at(i);
            const auto &rot = m_rotations.at(i);
            file << "p " << pos.x() << " " << pos.y() << " " << pos.z()
                 << std::endl;
            file << "r " << rot.scalar() << " " << rot.x() << " " << rot.y()
                 << " " << rot.z() << std::endl;
        }
        if (i == m_positions.size())
        {
            for (; i < m_rotations.size(); i++)
            {
                const auto &rot = m_rotations.at(i);
                file << "r " << rot.scalar() << " " << rot.x() << " " << rot.y()
                     << " " << rot.z() << std::endl;
            }
        }
        else
        {
            for (; i < m_positions.size(); i++)
            {
                const auto &pos = m_positions.at(i);
                file << "p " << pos.x() << " " << pos.y() << " " << pos.z()
                     << std::endl;
            }
        }
        for (const auto &m : m_marks)
        {
            file << "#mark " << m << std::endl;
        }

        std::cout << "Dump '" << dump_name << "' saved!" << std::endl;
        std::cout << m_marks.size() << " marks." << std::endl;
    }

    m_recording = !m_recording;
}

void CDataProvider::clear()
{
    std::unique_lock<std::mutex> guard{m_mutex};
    m_positions.clear();
    m_rotations.clear();
    m_marks.clear();
}

void CDataProvider::mark()
{
    std::unique_lock<std::mutex> guard{m_mutex};
    std::size_t m = m_positions.size() - 1;
    m_marks.emplace_back(m);
    std::cout << "Mark " << m << " placed! " << (m_marks.size() - 1) % 2
              << std::endl;
}

void CDataProvider::inputThread()
{
    std::string line;
    while (!m_stopped.load() && std::getline(std::cin, line))
    {
        std::unique_lock<std::mutex> guard{m_mutex};

        // if (line == "+")
        // {
        //     std::unique_lock<std::mutex> guard{m_mutex};
        //     m_positions.push_back(ContainerType());
        // }

        std::stringstream ss{line};

        std::string type;
        ss >> type;

        if (type == "p")
        {
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;

            ss >> x >> y >> z;

            if (ss)
            {
                m_positions.emplace_back(x, y, z);
            }
        }
        else if (type == "r")
        {
            float w = 0.0f;
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;

            ss >> w >> x >> y >> z;

            if (ss)
            {
                m_rotations.emplace_back(w, x, y, z);
            }
        }
    }
}

} // namespace viz
