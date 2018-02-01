cmake_minimum_required(VERSION 3.2)
project(Salsa20)

set(CMAKE_CXX_STANDARD 11)

set(SALSA20 Salsa20.h Salsa20.cpp)

set(SOURCE_FILES ${SALSA20} main.cpp)

add_executable(Salsa20 ${SOURCE_FILES})

#add_library(Salsa20 SHARED ${SOURCE_FILES})