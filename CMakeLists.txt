cmake_minimum_required(VERSION 2.6)
project(primenumbers)

add_library(sieve STATIC sieve.cpp)

add_executable(task1 task1.cpp)

add_executable(task2 task2.cpp)
target_link_libraries(task2 sieve)

add_executable(task3 task3.cpp)
target_link_libraries(task3 sieve)
