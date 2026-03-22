cmake_minimum_required(VERSION 3.16)

project(HarmonicVisualizer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"
    "src/*.ui"
)

add_executable(HarmonicVisualizer
    ${SOURCES}
)


target_link_libraries(HarmonicVisualizer
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
)
