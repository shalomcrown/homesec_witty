cmake_minimum_required(VERSION 2.6)
project(homesec_witty)

add_executable(homesec_witty main.cpp)

install(TARGETS homesec_witty RUNTIME DESTINATION bin)
