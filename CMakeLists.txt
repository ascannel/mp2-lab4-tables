cmake_minimum_required(VERSION 2.8)

project(mp2-lab4_tables)

include_directories(include)

add_executable(mp2-lab4_tables src/main.cpp include/iterator.hpp include/table.hpp)