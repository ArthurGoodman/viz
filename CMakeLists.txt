cmake_minimum_required(VERSION 3.0.0)
project(viz)

################################################################################
# CMake options
################################################################################

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

################################################################################
# Compiler settings
################################################################################

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(FLAGS "-Wall -Wextra -Wunused-variable -Wno-comment")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")

################################################################################
# Third-party dependencies
################################################################################

# Qt5
find_package(Qt5 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    )

# Threads
find_package(Threads REQUIRED)

################################################################################
# Targets
################################################################################

set(VIZ ${PROJECT_NAME})

add_subdirectory(src)
