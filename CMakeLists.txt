cmake_minimum_required(VERSION 3.3)
project(alectryon)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

find_package(OpenCV REQUIRED)

set(SOURCE_FILES src/main.c src/utilities/utilities.c src/utilities/utilities.h src/utilities/file_manager/file_manager.c src/utilities/file_manager/file_manager.h)
add_executable(alectryon ${SOURCE_FILES})
target_link_libraries(alectryon ${OpenCV_LIBS})