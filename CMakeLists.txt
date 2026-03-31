cmake_minimum_required(VERSION 3.16)
project(TaskScheduler)

# Find Qt6 - Ensure this matches your installed version
# Added "Charts" here
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

set(CMAKE_AUTOMOC ON) # Handles Q_OBJECT macros
set(CMAKE_AUTOUIC ON) # Handles .ui to ui_MainWindow.h conversion
set(CMAKE_AUTORCC ON) # Handles resources

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

# Added "Qt6::Charts" here
target_link_libraries(TaskScheduler PRIVATE Qt6::Widgets Qt6::Charts)