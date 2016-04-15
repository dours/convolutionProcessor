cmake_minimum_required(VERSION 3.4)
project(ConvolutionProc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lsystemc -Wall")

set(SOURCE_FILES main.cpp Unit.cpp pipeline_sc.cpp ProcessorState.cpp instruction.cpp LordOfTheHeaders.h)
add_executable(ConvolutionProc ${SOURCE_FILES})