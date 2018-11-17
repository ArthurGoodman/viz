set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${VIZ}
    main.cpp
    DataProvider.cpp
    Renderer.cpp
    View.cpp
    resources/resources.qrc
    )

target_include_directories(${VIZ}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(${VIZ}
    PRIVATE Qt5::Core
    PRIVATE Qt5::Gui
    PRIVATE Qt5::Widgets
    PRIVATE Threads::Threads
    )
