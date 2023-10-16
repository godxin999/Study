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

add_executable(TestRTTR main.cpp)

set(DEPENDS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../depends)
set(RTTR_DIR ${DEPENDS_PATH}/rttr/cmake)

target_include_directories(TestRTTR PUBLIC ${DEPENDS_PATH}/rttr/include)

find_package(RTTR CONFIG REQUIRED COMPONENTS Core)
target_link_libraries(TestRTTR PUBLIC RTTR::Core)

add_custom_command(TARGET TestRTTR POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:RTTR::Core>
		$<TARGET_FILE_DIR:TestRTTR>)