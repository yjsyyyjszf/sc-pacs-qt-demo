find_package(Qt5 COMPONENTS Core REQUIRED)

if(APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework PCSC")
endif()

if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS -I/usr/include/PCSC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
endif()

set_property(TARGET Qt5::Core APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS $<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS qpcscxx.h)
set(SOURCES pcsc.cpp)
set(CMAKE_AUTOMOC false)

add_library(qpcscxx STATIC ${HEADERS} ${SOURCES})
if(APPLE)
    target_link_libraries(qpcscxx PUBLIC Qt5::Core)
endif()
if(UNIX AND NOT APPLE)
    target_link_libraries(qpcscxx PUBLIC Qt5::Core pcsclite)
endif()