﻿# 设置glfw所在目录
set(GLFW_SOURCE_DIR ../../depends/glfw-3.3-3.4)

#--------------------------------------------------------------------
# Detect and select backend APIs
#--------------------------------------------------------------------
if (GLFW_USE_WAYLAND)
    set(_GLFW_WAYLAND 1)
    message(STATUS "Using Wayland for window creation")
elseif (GLFW_USE_OSMESA)
    set(_GLFW_OSMESA 1)
    message(STATUS "Using OSMesa for headless context creation")
elseif (WIN32)
    set(_GLFW_WIN32 1)
    add_definitions(-D _GLFW_WIN32)
    link_libraries(opengl32)
    message(STATUS "Using Win32 for window creation")
elseif (APPLE)
    set(_GLFW_COCOA 1)
    add_definitions(-D _GLFW_COCOA)
    message(STATUS "Using Cocoa for window creation")
elseif (UNIX)
    set(_GLFW_X11 1)
    message(STATUS "Using X11 for window creation")
else()
    message(FATAL_ERROR "No supported platform was detected")
endif()

#--------------------------------------------------------------------
# Use Cocoa for window creation and NSOpenGL for context creation
#--------------------------------------------------------------------
if (_GLFW_COCOA)

    list(APPEND glfw_LIBRARIES
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreFoundation")

    set(glfw_PKG_DEPS "")
    set(glfw_PKG_LIBS "-framework Cocoa -framework IOKit -framework CoreFoundation")
endif()


set(common_HEADERS ${GLFW_SOURCE_DIR}/src/internal.h ${GLFW_SOURCE_DIR}/src/mappings.h
        "${GLFW_SOURCE_DIR}/src/glfw_config.h"
        "${GLFW_SOURCE_DIR}/include/GLFW/glfw3.h"
        "${GLFW_SOURCE_DIR}/include/GLFW/glfw3native.h")
set(common_SOURCES ${GLFW_SOURCE_DIR}/src/context.c ${GLFW_SOURCE_DIR}/src/init.c ${GLFW_SOURCE_DIR}/src/input.c ${GLFW_SOURCE_DIR}/src/monitor.c ${GLFW_SOURCE_DIR}/src/vulkan.c ${GLFW_SOURCE_DIR}/src/window.c)

include_directories("${GLFW_SOURCE_DIR}/include" "${GLFW_SOURCE_DIR}/deps")


set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h"
        "${GLFW_SOURCE_DIR}/deps/glad_gl.c")
set(GLAD_VULKAN "${GLFW_SOURCE_DIR}/deps/glad/vulkan.h"
        "${GLFW_SOURCE_DIR}/deps/glad_vulkan.c")
set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
        "${GLFW_SOURCE_DIR}/deps/getopt.c")
set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
        "${GLFW_SOURCE_DIR}/deps/tinycthread.c")


if (_GLFW_COCOA)
    set(glfw_headers ${common_HEADERS} ${GLFW_SOURCE_DIR}/src/cocoa_platform.h ${GLFW_SOURCE_DIR}/src/cocoa_joystick.h
            ${GLFW_SOURCE_DIR}/src/posix_thread.h ${GLFW_SOURCE_DIR}/src/nsgl_context.h ${GLFW_SOURCE_DIR}/src/egl_context.h ${GLFW_SOURCE_DIR}/src/osmesa_context.h)
    set(glfw_sources ${common_SOURCES} ${GLAD_GL} ${GLFW_SOURCE_DIR}/src/cocoa_init.m ${GLFW_SOURCE_DIR}/src/cocoa_joystick.m
            ${GLFW_SOURCE_DIR}/src/cocoa_monitor.m ${GLFW_SOURCE_DIR}/src/cocoa_window.m ${GLFW_SOURCE_DIR}/src/cocoa_time.c ${GLFW_SOURCE_DIR}/src/posix_thread.c
            ${GLFW_SOURCE_DIR}/src/nsgl_context.m ${GLFW_SOURCE_DIR}/src/egl_context.c ${GLFW_SOURCE_DIR}/src/osmesa_context.c)
    link_libraries(${glfw_LIBRARIES})
elseif (_GLFW_WIN32)
    set(glfw_headers ${common_HEADERS} ${GLFW_SOURCE_DIR}/src/win32_platform.h ${GLFW_SOURCE_DIR}/src/win32_joystick.h
            ${GLFW_SOURCE_DIR}/src/wgl_context.h ${GLFW_SOURCE_DIR}/src/egl_context.h ${GLFW_SOURCE_DIR}/src/osmesa_context.h)
    set(glfw_sources ${common_SOURCES} ${GLAD_GL} ${GLFW_SOURCE_DIR}/src/win32_init.c ${GLFW_SOURCE_DIR}/src/win32_joystick.c
            ${GLFW_SOURCE_DIR}/src/win32_monitor.c ${GLFW_SOURCE_DIR}/src/win32_time.c ${GLFW_SOURCE_DIR}/src/win32_thread.c ${GLFW_SOURCE_DIR}/src/win32_window.c
            ${GLFW_SOURCE_DIR}/src/wgl_context.c ${GLFW_SOURCE_DIR}/src/egl_context.c ${GLFW_SOURCE_DIR}/src/osmesa_context.c)
elseif (_GLFW_X11)
    set(glfw_headers ${common_HEADERS} x11_platform.h xkb_unicode.h posix_time.h
            posix_thread.h glx_context.h egl_context.h osmesa_context.h)
    set(glfw_sources ${common_SOURCES} x11_init.c x11_monitor.c x11_window.c
            xkb_unicode.c posix_time.c posix_thread.c glx_context.c
            egl_context.c osmesa_context.c)
elseif (_GLFW_WAYLAND)
    set(glfw_headers ${common_HEADERS} wl_platform.h
            posix_time.h posix_thread.h xkb_unicode.h egl_context.h
            osmesa_context.h)
    set(glfw_sources ${common_SOURCES} wl_init.c wl_monitor.c wl_window.c
            posix_time.c posix_thread.c xkb_unicode.c
            egl_context.c osmesa_context.c)

    ecm_add_wayland_client_protocol(glfw_sources
            PROTOCOL
            "${WAYLAND_PROTOCOLS_PKGDATADIR}/stable/xdg-shell/xdg-shell.xml"
            BASENAME xdg-shell)
    ecm_add_wayland_client_protocol(glfw_sources
            PROTOCOL
            "${WAYLAND_PROTOCOLS_PKGDATADIR}/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml"
            BASENAME xdg-decoration)
    ecm_add_wayland_client_protocol(glfw_sources
            PROTOCOL
            "${WAYLAND_PROTOCOLS_PKGDATADIR}/stable/viewporter/viewporter.xml"
            BASENAME viewporter)
    ecm_add_wayland_client_protocol(glfw_sources
            PROTOCOL
            "${WAYLAND_PROTOCOLS_PKGDATADIR}/unstable/relative-pointer/relative-pointer-unstable-v1.xml"
            BASENAME relative-pointer-unstable-v1)
    ecm_add_wayland_client_protocol(glfw_sources
            PROTOCOL
            "${WAYLAND_PROTOCOLS_PKGDATADIR}/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml"
            BASENAME pointer-constraints-unstable-v1)
    ecm_add_wayland_client_protocol(glfw_sources
            PROTOCOL
            "${WAYLAND_PROTOCOLS_PKGDATADIR}/unstable/idle-inhibit/idle-inhibit-unstable-v1.xml"
            BASENAME idle-inhibit-unstable-v1)
elseif (_GLFW_OSMESA)
    set(glfw_headers ${common_HEADERS} null_platform.h null_joystick.h
            posix_time.h posix_thread.h osmesa_context.h)
    set(glfw_sources ${common_SOURCES} null_init.c null_monitor.c null_window.c
            null_joystick.c posix_time.c posix_thread.c osmesa_context.c)
endif()

if (_GLFW_X11 OR _GLFW_WAYLAND)
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(glfw_headers ${glfw_headers} linux_joystick.h)
        set(glfw_sources ${glfw_sources} linux_joystick.c)
    else()
        set(glfw_headers ${glfw_headers} null_joystick.h)
        set(glfw_sources ${glfw_sources} null_joystick.c)
    endif()
endif()

# Workaround for CMake not knowing about .m files before version 3.16
if (CMAKE_VERSION VERSION_LESS "3.16" AND APPLE)
    set_source_files_properties(cocoa_init.m cocoa_joystick.m cocoa_monitor.m
            cocoa_window.m nsgl_context.m PROPERTIES
            LANGUAGE C)
endif()

#add_library(glfw ${glfw_sources} ${glfw_headers})
#set_target_properties(glfw PROPERTIES
#        OUTPUT_NAME ${GLFW_LIB_NAME}
#        VERSION ${GLFW_VERSION_MAJOR}.${GLFW_VERSION_MINOR}
#        SOVERSION ${GLFW_VERSION_MAJOR}
#        POSITION_INDEPENDENT_CODE ON
#        FOLDER "GLFW3")

#if (CMAKE_VERSION VERSION_EQUAL "3.1.0" OR
#        CMAKE_VERSION VERSION_GREATER "3.1.0")
#
#    set_target_properties(glfw PROPERTIES C_STANDARD 99)
#else()
#    # Remove this fallback when removing support for CMake version less than 3.1
#    target_compile_options(glfw PRIVATE
#            "$<$<C_COMPILER_ID:AppleClang>:-std=c99>"
#            "$<$<C_COMPILER_ID:Clang>:-std=c99>"
#            "$<$<C_COMPILER_ID:GNU>:-std=c99>")
#endif()

#target_compile_definitions(glfw PRIVATE _GLFW_USE_CONFIG_H)
#target_include_directories(glfw PUBLIC
#        "$<BUILD_INTERFACE:${GLFW_SOURCE_DIR}/include>"
#        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
#target_include_directories(glfw PRIVATE
#        "${GLFW_SOURCE_DIR}/src"
#        "${GLFW_BINARY_DIR}/src"
#        ${glfw_INCLUDE_DIRS})
#target_link_libraries(glfw PRIVATE ${glfw_LIBRARIES})

# Make GCC warn about declarations that VS 2010 and 2012 won't accept for all
# source files that VS will build (Clang ignores this because we set -std=c99)
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set_source_files_properties(context.c init.c input.c monitor.c vulkan.c
            window.c win32_init.c win32_joystick.c
            win32_monitor.c win32_time.c win32_thread.c
            win32_window.c wgl_context.c egl_context.c
            osmesa_context.c PROPERTIES
            COMPILE_FLAGS -Wdeclaration-after-statement)
endif()

# Enable a reasonable set of warnings
#if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
#        CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
#        CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
#
#    if (CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
#        # Tell Clang-CL that this is a Clang flag
#        target_compile_options(glfw PRIVATE "/clang:-Wall")
#    else()
#        target_compile_options(glfw PRIVATE "-Wall")
#    endif()
#endif()
#
#if (WIN32)
#    target_compile_definitions(glfw PRIVATE _UNICODE)
#endif()

# HACK: When building on MinGW, WINVER and UNICODE need to be defined before
# the inclusion of stddef.h (by glfw3.h), which is itself included before
# win32_platform.h.  We define them here until a saner solution can be found
# NOTE: MinGW-w64 and Visual C++ do /not/ need this hack.
#if (MINGW)
#    target_compile_definitions(glfw PRIVATE UNICODE WINVER=0x0501)
#endif()

#target_sources(${glfw_sources} ${glfw_headers})