cmake_minimum_required(VERSION 3.20.0)
project(module_app) 


set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # does not produce the json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works


include_directories(include .)

add_executable(module_app example/main.cpp)

target_include_directories(module_app PUBLIC . include)

