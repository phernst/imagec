cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME ImageC)
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(LIBTORCH_PATH CACHE PATH "Path to LibTorch directory")
list(APPEND CMAKE_PREFIX_PATH ${LIBTORCH_PATH})

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
find_package(Torch REQUIRED)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND CMAKE_EXE_LINKER_FLAGS "-fuse-ld=gold")
endif()

set(SRC_FILES
    main.cpp
    mainwindow.cpp
    image.cpp
    imagewindow.cpp
)

set(QT_FORMS
    imagewindow.ui
)

set(MOC_FILES
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${QT_FORMS} ${MOC_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    ${TORCH_LIBRARIES}
)