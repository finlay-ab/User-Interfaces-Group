cmake_minimum_required(VERSION 3.16)

project(COMP2811_UI_Project VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify Qt installation path
set(CMAKE_PREFIX_PATH "C:/QtNew/6.8.1/mingw_64/lib/cmake")

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add executable and source files
qt_add_executable(quaketool
        src/main.cpp
        src/widgets/UnifiedWidget.cpp
        include/widgets/UnifiedWidget.hpp
)

# Link Qt libraries
target_link_libraries(quaketool PRIVATE Qt6::Widgets)

# Ensure headers are processed by MOC
set_target_properties(quaketool PROPERTIES
        AUTOMOC ON
        WIN32_EXECUTABLE ON
)
