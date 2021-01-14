cmake_minimum_required(VERSION 3.1.0)
project(sc_pacs_demo)

set(SCPACS_VERSION 1.0)

#configure_file (
#  "${PROJECT_SOURCE_DIR}/src/settings.h.in"
#  "${PROJECT_BINARY_DIR}/src/settings.h"
#)

# tell where qt is
if(APPLE)
    set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.14.1/clang_64/lib/cmake)
endif()

if(WIN32)
    set(CMAKE_PREFIX_PATH C:/Qt/Qt5.14.1/5.14.1/mingw73_32/lib/cmake)
endif()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_subdirectory(qpcscxx)
add_subdirectory(gui)
