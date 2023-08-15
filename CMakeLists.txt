cmake_minimum_required(VERSION 3.20.0)
project(module_app) 

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # does not produce the json file
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

## APP
SET(CMAKE_CXX_FLAGS "-rdynamic")
include_directories(include .)
add_executable(module_app example/main.cpp)
target_include_directories(module_app PUBLIC . include)
set_property(TARGET module_app PROPERTY CXX_STANDARD 17)


## LIB
add_library(TestModule SHARED example/modules/CompiledModule/TestModule.cpp)
target_include_directories(TestModule PUBLIC . include)

set_property(TARGET TestModule PROPERTY CXX_STANDARD 17)
set_property(TARGET TestModule PROPERTY CMAKE_CXX_FLAGS "-fPIC")

