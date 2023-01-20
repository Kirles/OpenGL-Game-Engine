cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set( CMAKE_CXX_COMPILER "/pathto/g++" )
set( CMAKE_C_COMPILER "/pathto/gcc" )

project(TankBattle)

add_executable(TankBattle src/main.cpp)