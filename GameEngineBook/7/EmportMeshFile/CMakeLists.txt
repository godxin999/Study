cmake_minimum_required(VERSION 3.17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake;${CMAKE_MODULE_PATH}")

project(ExportMeshFile LANGUAGES C CXX)

if (WIN32)
    add_compile_options(/wd4819)#忽略保存为unicode警告
endif ()

include(glfw)

add_executable(ExportMeshFile)

set(DEPENDS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../depends)

target_include_directories(ExportMeshFile PUBLIC ${DEPENDS_PATH}/glm/include)
target_include_directories(ExportMeshFile PUBLIC ${DEPENDS_PATH}/stb/include)
#target_include_directories(ExportMeshFile PUBLIC ${DEPENDS_PATH}/timetool/include)

set(sources main.cpp texture2d.cpp ${glfw_sources} VertexData.h ShaderSource.h texture2d.h)

target_sources(ExportMeshFile PUBLIC ${sources})

add_custom_target(run COMMAND $<TARGET_FILE:ExportMeshFile>)
