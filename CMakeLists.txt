cmake_minimum_required(VERSION 3.16)
project(notebook LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
)
qt_standard_project_setup()

set(PROJECT_UI_SOURCES
    ui/main.cpp
    ui/notebook.ui
    ui/notebook.h
    ui/notebook.cpp
    ui/group_dashboard.ui
    ui/group_dashboard.h
    ui/group_dashboard.cpp
    ui/question_dashboard.ui
    ui/question_dashboard.h
    ui/question_dashboard.cpp
)

set(PROJECT_SOURCES
    src/xn.pb.cc
    src/notebook_utils.cpp
    src/notebook_impl.cpp
    src/question_answer_impl.cpp
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_UI_SOURCES} ${PROJECT_SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    dep/include
    src/)

target_link_directories(${PROJECT_NAME}
    PRIVATE
    dep/lib/${CMAKE_BUILD_TYPE}
)


target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        abseil_dll
        libprotobufd
        libprotobuf-lited
)

