# cmake_minimum_required(VERSION 3.16)
# project(PollutantOverviewExample)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
# set(CMAKE_AUTOUIC ON)

# add_executable(PollutantOverviewExample
#     pollutantoverviewwidget.cpp
#     pollutantoverviewwidget.hpp
#     main.cpp
#     utils.hpp
#     utils.cpp
    

    
# )

# target_link_libraries(PollutantOverviewExample PRIVATE Qt6::Widgets Qt6::Charts)

cmake_minimum_required(VERSION 3.16)

project(PollutantOverview VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts)

add_executable(PollutantOverview
    main.cpp
    pollutantoverviewwidget.cpp
    pollutantoverviewwidget.hpp
)

target_link_libraries(PollutantOverview PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Charts
)