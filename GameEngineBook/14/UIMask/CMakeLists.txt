cmake_minimum_required(VERSION 3.17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake;${CMAKE_MODULE_PATH}")

project(UIMask LANGUAGES C CXX)

if (WIN32)
    add_compile_options(/wd4819)#忽略保存为unicode警告
endif ()

include(glad)

add_executable(UIMask)

set(DEPENDS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../depends)
set(RTTR_DIR ${DEPENDS_PATH}/rttr/cmake)
set(glfw3_DIR ${DEPENDS_PATH}/glfw/lib/cmake/glfw3)
set(freetype_DIR ${DEPENDS_PATH}/freetype/lib/cmake/freetype)
set(spdlog_DIR ${DEPENDS_PATH}/spdlog/lib/cmake/spdlog)

target_include_directories(UIMask PUBLIC ${DEPENDS_PATH}/GLFW/include)
target_include_directories(UIMask PUBLIC ${DEPENDS_PATH}/glm/include)
target_include_directories(UIMask PUBLIC ${DEPENDS_PATH}/stb/include)
target_include_directories(UIMask PUBLIC ${DEPENDS_PATH}/rapidxml/include)
target_include_directories(UIMask PUBLIC ${DEPENDS_PATH}/rttr/include)
target_include_directories(UIMask PUBLIC ${DEPENDS_PATH}/glad/include)
target_include_directories(UIMask PUBLIC ${DEPENDS_PATH}/freetype/include)
target_include_directories(UIMask PUBLIC ${DEPENDS_PATH}/spdlog/include)
target_include_directories(UIMask PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)

find_package(RTTR CONFIG REQUIRED COMPONENTS Core)
target_link_libraries(UIMask PUBLIC RTTR::Core)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(UIMask PUBLIC glfw)
find_package(freetype CONFIG REQUIRED)
target_link_libraries(UIMask PUBLIC freetype)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(UIMask PUBLIC spdlog::spdlog)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS source/*.h source/*.cpp)
file(GLOB_RECURSE example_sources CONFIGURE_DEPENDS example/*.cpp example/*.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources} ${example_sources})
target_sources(UIMask PUBLIC ${sources} ${glad_sources} ${example_sources})

add_custom_command(TARGET UIMask POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:RTTR::Core>
		$<TARGET_FILE_DIR:UIMask>)