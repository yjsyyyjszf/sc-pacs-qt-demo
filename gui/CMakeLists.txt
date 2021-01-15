find_package(Qt5 COMPONENTS Core Widgets LinguistTools Svg Xml REQUIRED)

if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE sc-pacs-demo.icns)
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${SCPACS_VERSION})
    set(MACOSX_BUNDLE_COPYRIGHT "© Copyright 2020 Sergey Stolyarov")
    set(MACOSX_BUNDLE_BUNDLE_NAME "SC PACS DEMO")

    find_package(Qt5 COMPONENTS MacExtras REQUIRED)
    set(myApp_ICON ${CMAKE_CURRENT_SOURCE_DIR}/../resources/sc-pacs-demo.icns)
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/../resources/sc-pacs-demo.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework PCSC")
endif()

set_property(TARGET Qt5::Core APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS $<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)
set(CMAKE_CXX_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR}/../qpcscxx)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${PROJECT_BINARY_DIR}/qpcscxx")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(HEADERS utils.h)

set(SOURCES main.cpp
    utils.cpp)

## hack to add generated qm-files into compiled binary
#set(TRANSLATIONS_QRC "translations.qrc")
#configure_file(${TRANSLATIONS_QRC} ${TRANSLATIONS_QRC} COPYONLY)

#qt5_add_resources(RESOURCES ../resources/application.qrc ${CMAKE_BINARY_DIR}/src/${TRANSLATIONS_QRC})

#set(LANGUAGES scpacs-ru)
#foreach (LANGUAGE ${LANGUAGES})
#    qt5_add_translation(TRANSLATIONS ${LANGUAGE}.ts)
#    list(APPEND QMS ${LANGUAGE}.qm)
#endforeach()

set(ALL_SOURCES ${SOURCES} ${HEADERS} ${TRANSLATIONS} ${RESOURCES})

if(APPLE)
    add_executable(sc-pacs-demo MACOSX_BUNDLE ${myApp_ICON}  ${ALL_SOURCES})
endif()

if(WIN32)
    add_executable(sc-pacs-demo WIN32 ${ALL_SOURCES})
# special case for debugging console
#    add_executable(sc-pacs-demo ${ALL_SOURCES})
endif()

if(UNIX AND NOT APPLE)
    add_executable(sc-pacs-demo ${ALL_SOURCES})
endif()

if(APPLE)
    target_link_libraries(sc-pacs-demo Qt5::Widgets Qt5::Svg Qt5::Xml Qt5::MacExtras)
endif()
if(NOT APPLE)
    target_link_libraries(sc-pacs-demo Qt5::Widgets Qt5::Svg Qt5::Xml qpcscxx)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
install(TARGETS sc-pacs-demo
    RUNTIME DESTINATION /usr/bin
)
endif()
