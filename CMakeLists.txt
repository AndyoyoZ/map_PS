cmake_minimum_required(VERSION 2.8)

#setOpenCV_DIR
set(OpenCV_DIR "/home/andyoyo/opencv/opencv-master/build")

project(map_PS)

find_package( OpenCV REQUIRED)

#print OpenCV_VERSION on terminal

message(STATUS "OpenCV --version: " ${OpenCV_VERSION})

if(NOT WIN32)
ADD_DEFINITIONS("-std=c++0x -O3")
endif(NOT WIN32)

include_directories(src) 
FILE(GLOB_RECURSE sourcefiles "src/*.cpp")

add_executable(map_PS ${sourcefiles} )
target_link_libraries(map_PS ${OpenCV_LIBS})
