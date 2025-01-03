cmake_minimum_required(VERSION 3.29)
project(Matam_HW3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Matam_HW3
    main.cpp
    Person.cpp
    Person.h
    SortedList.h
    Task.cpp
    Task.h
    TaskManager.h)
