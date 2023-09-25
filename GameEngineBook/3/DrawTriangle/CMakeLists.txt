cmake_minimum_required(VERSION 3.17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake;${CMAKE_MODULE_PATH}")

project(DrawTriangle LANGUAGES C CXX)

if (WIN32)
    add_compile_options(/wd4819)#忽略保存为unicode警告
endif ()

include(glfw)

add_executable(DrawTriangle)

target_include_directories(DrawTriangle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../depends)

set(sources main.cpp ${glfw_sources} VertexData.h ShaderSource.h)

target_sources(DrawTriangle PUBLIC ${sources})

add_custom_target(run COMMAND $<TARGET_FILE:DrawTriangle>)