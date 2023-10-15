cmake_minimum_required(VERSION 3.17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake;${CMAKE_MODULE_PATH}")

project(TestRTTR LANGUAGES C CXX)

if (WIN32)
    add_compile_options(/wd4819)#忽略保存为unicode警告
endif ()

add_executable(TestRTTR)

set(DEPENDS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../depends)

target_include_directories(TestRTTR PUBLIC ${DEPENDS_PATH}/rttr/src)

file(GLOB_RECURSE rttr_source CONFIGURE_DEPENDS ${DEPENDS_PATH}/rttr/src/rttr/*.cpp ${DEPENDS_PATH}/rttr/src/rttr/*.h)

target_sources(TestRTTR PUBLIC main.cpp ${rttr_source})