cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME ImageC)
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

set(SRC_FILES
    main.cpp
    mainwindow.cpp
)

set(QT_FORMS
)

set(MOC_FILES
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${QT_FORMS} ${MOC_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)