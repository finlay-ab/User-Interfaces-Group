cmake_minimum_required(VERSION 3.16)

project(cwk2 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Charts)



qt_standard_project_setup()

qt_add_executable(quaketool
    main.cpp
    quake.cpp
    quake.hpp
    dataset.cpp
    dataset.hpp
    model.cpp
    window.cpp
    stats.cpp
    nadia/complianceDashboard.cpp  
    daniel/fluorinated_compounds_page/window.cpp
    daniel/physical_pollution_page/window.cpp
    daniel/fluorinated_compounds_page/line_chart.cpp
    daniel/physical_pollution_page/pie_chart.cpp
    rameesah/overviewWidget.cpp
    rameesah/overviewWidget.hpp
    rameesah/PCBMonitor.cpp
    rameesah/PCBMonitor.hpp
    tayshan/dashwidge.cpp
    tayshan/dashwidge.hpp
    tayshan/datawidge.cpp
    tayshan/datawidge.hpp
    tayshan/window.cpp
    tayshan/window.hpp
)

target_link_libraries(quaketool PRIVATE Qt6::Widgets)
target_link_libraries(quaketool PRIVATE Qt6::Charts)

set_target_properties(quaketool PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)