cmake_minimum_required(VERSION 3.27)
project(opencv_learn)
set(OpenCV_DIR /opt/homebrew/Cellar/opencv/4.9.0_2/lib/)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)
include_directories(/opt/homebrew/Cellar/opencv/4.9.0_2/include)
add_executable(opencv_learn main.cpp)

target_link_libraries(opencv_learn ${OpenCV_LIBS})
