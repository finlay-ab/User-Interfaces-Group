cmake_minimum_required(VERSION 3.16)

project(fluoride VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Charts)
qt_standard_project_setup()

qt_add_executable(fluoride
    main.cpp
    line_chart.cpp
    window.cpp
    info.cpp
)

target_link_libraries(fluoride PRIVATE Qt6::Widgets)
target_link_libraries(fluoride PRIVATE Qt6::Charts)

set_target_properties(fluoride PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)
