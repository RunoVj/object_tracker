cmake_minimum_required(VERSION 2.8)

project(object_detector)

add_executable(${PROJECT_NAME}
    "main.cpp"
    "harrisdetector.cpp"
    "videoprocessor.cpp"
    "frameprocessor"
    "featuretracker")

find_package( OpenCV REQUIRED )
target_link_libraries( object_detector ${OpenCV_LIBS} )
