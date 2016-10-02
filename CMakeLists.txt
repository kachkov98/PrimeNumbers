cmake_minimum_required(VERSION 2.6)
project(primenumbers)

add_executable(primenumbers main.cpp)

install(TARGETS primenumbers RUNTIME DESTINATION bin)
