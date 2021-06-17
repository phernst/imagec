cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME imagec)
project(${PROJECT_NAME})

set(SRC_FILES
    main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
