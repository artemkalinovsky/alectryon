cmake_minimum_required(VERSION 3.3)
project(alectryon)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package( OpenCV REQUIRED )

set(SOURCE_FILES main.c)
add_executable(alectryon ${SOURCE_FILES})
target_link_libraries( alectryon ${OpenCV_LIBS} )