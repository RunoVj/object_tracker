cmake_minimum_required(VERSION 2.8)

project(Test)

add_executable(${PROJECT_NAME}
    "main.cpp"
    "harrisdetector.cpp")

find_package( OpenCV REQUIRED )
target_link_libraries( Test ${OpenCV_LIBS} )
