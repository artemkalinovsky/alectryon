cmake_minimum_required(VERSION 3.3)
project(alectryon)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

find_package(OpenCV REQUIRED)

set(SOURCE_FILES src/main.c src/constants/constants.h src/utilities/file_manager/file_manager.c src/utilities/file_manager/file_manager.h src/constants/os_x_constants.h src/constants/linux_constants.h src/utilities/motion_detector/motion_detector.c src/utilities/motion_detector/motion_detector.h)
add_executable(alectryon ${SOURCE_FILES})
target_link_libraries(alectryon ${OpenCV_LIBS})