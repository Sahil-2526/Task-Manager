cmake_minimum_required(VERSION 3.16)
project(TaskScheduler)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Gui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(TaskScheduler
    main.cpp
    MainWindow.cpp
    MainWindow.h
    Scheduler.cpp
    Scheduler.h
    Task.cpp
    Task.h
    Utils.cpp
    Utils.h
)

target_link_libraries(TaskScheduler 
    PRIVATE 
    Qt6::Widgets 
    Qt6::Charts 
    Qt6::Gui
)