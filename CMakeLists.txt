cmake_minimum_required(VERSION 3.30)
project(n4)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} main.cpp
        VideoResolution.cpp
        VideoResolution.h)

find_package(OpenCV REQUIRED)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
