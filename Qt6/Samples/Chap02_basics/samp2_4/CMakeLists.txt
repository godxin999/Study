cmake_minimum_required(VERSION 3.5)#需要的CMake最低版本

project(samp2_4 VERSION 0.1 LANGUAGES CXX)#项目版本0.1，编程语言为C++

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)#UIC可以被自动执行
set(CMAKE_AUTOMOC ON)#MOC可以被自动执行
set(CMAKE_AUTORCC ON)#RCC可以被自动执行

set(CMAKE_CXX_STANDARD 17)#编译器需要满足的C++语言标准，为C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)#要求编译器满足C++标准

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
#导入Qt6::Widgets模块
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
#设置源文件列表如下
set(PROJECT_SOURCES
        main.cpp
        dialog.cpp
        dialog.h
        dialog.ui
        res.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6) #如果是Qt6以上版本
    qt_add_executable(samp2_4 #创建可执行文件samp2_4
        MANUAL_FINALIZATION #可选参数，手动结束创建目标的过程
        ${PROJECT_SOURCES} #文件列表来源于变量PROJECT_SOURCES
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET samp2_4 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(samp2_4 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(samp2_4
            ${PROJECT_SOURCES}
        )
    endif()
endif()
#在链接生成目标samp2_4时，需要利用导入的Qt::Widgets模块
target_link_libraries(samp2_4 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(samp2_4 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS samp2_4
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(samp2_4) #最后生成可执行文件samp2_4
endif()
