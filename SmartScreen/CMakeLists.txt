cmake_minimum_required(VERSION 3.16)
project(SmartScreen VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 交叉编译时设置工具链（T113）
# 使用方式：cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-t113.cmake
# 本地Ubuntu编译直接 cmake ..

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    Network
    Gui
)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/PomodoroPage.cpp
    src/WeatherPage.cpp
    src/SettingsPage.cpp
    src/WeatherManager.cpp
    src/PageManager.cpp
)

set(HEADERS
    include/MainWindow.h
    include/PomodoroPage.h
    include/WeatherPage.h
    include/SettingsPage.h
    include/WeatherManager.h
    include/PageManager.h
)

add_executable(SmartScreen ${SOURCES} ${HEADERS})

target_link_libraries(SmartScreen
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::Gui
)

# T113嵌入式平台优化
if(CMAKE_CROSSCOMPILING)
    target_compile_definitions(SmartScreen PRIVATE EMBEDDED_TARGET)
    message(STATUS "Cross-compiling for T113")
endif()
