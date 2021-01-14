find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

if(APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework PCSC")
endif()

if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS -I/usr/include/PCSC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpcsclite")
endif()

set_property(TARGET Qt5::Core APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS $<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>)

set(HEADERS qpcscxx.h)
set(SOURCES pcsc.cpp)
set(CMAKE_AUTOMOC false)

add_library(qpcscxx ${HEADERS} ${SOURCES})
target_link_libraries(qpcscxx Qt5::Core)