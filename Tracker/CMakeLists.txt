cmake_minimum_required(VERSION 3.10)
project(Tracker)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp headers/edgeDetector.h source/edgeDetector.cpp headers/camera.h source/camera.cpp headers/detector.h headers/transformations.h data/subpoint.h data/line.h headers/poseEstimation.h source/poseEstimation.cpp)

configure_file(resources/lena resources/lena COPYONLY)
configure_file(resources/movie resources/movie COPYONLY)
configure_file(resources/stuff resources/stuff COPYONLY)

add_executable(Tracker ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(Tracker ${OpenCV_LIBS})