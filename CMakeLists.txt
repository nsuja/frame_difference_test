cmake_minimum_required(VERSION 2.8)

project(FrameDifferenceTest C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

file(GLOB source FrameDifferenceTest.cpp)

file(GLOB_RECURSE bgs_src ./package_bgs/*.cpp ./package_bgs/*.c ./package_analysis/*.cpp)
file(GLOB_RECURSE bgs_inc ./package_bgs/*.h ./package_analysis/*.h)

include_directories(${CMAKE_SOURCE_DIR})

add_library(bgs SHARED ${bgs_src})
target_link_libraries(bgs ${OpenCV_LIBS})
set_property(TARGET bgs PROPERTY PUBLIC_HEADER ${bgs_inc})

add_executable(FrameDifferenceTest ${source})
target_link_libraries(FrameDifferenceTest ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} bgs)
