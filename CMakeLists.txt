cmake_minimum_required(VERSION 3.16)
project(ModulesDispatcher LANGUAGES CXX)

# Include-only library: expose headers path to parent project.
set(ModulesDispatcher_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    CACHE INTERNAL "ModulesDispatcher include root"
)
