cmake_minimum_required(VERSION 3.1.0)
project(sc_pacs_demo)

set(SCPACS_VERSION 1.0)

#configure_file (
#  "${PROJECT_SOURCE_DIR}/src/settings.h.in"
#  "${PROJECT_BINARY_DIR}/src/settings.h"
#)

# tell where qt is
if(APPLE)
    set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.15.2/clang_64/lib/cmake)
endif()

if(WIN32)
    set(CMAKE_PREFIX_PATH C:/Qt/Qt5.15.2/5.15.2/mingw73_32/lib/cmake)
endif()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(qpcscxx)
add_subdirectory(gui)
