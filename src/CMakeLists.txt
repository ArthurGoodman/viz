set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${VIZ}
    viz.cpp
    )

target_link_libraries(${VIZ}
    PRIVATE Qt5::Core
    PRIVATE Qt5::Gui
    PRIVATE Qt5::Widgets
    PRIVATE Threads::Threads
    )
