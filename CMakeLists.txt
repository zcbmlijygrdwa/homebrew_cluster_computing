cmake_minimum_required(VERSION 3.4.1)

project(demo)


include_directories(
include
)

add_executable(example_master example_master.cpp)
target_link_libraries(example_master
)

add_executable(example_slave example_slave.cpp)
target_link_libraries(example_slave
)
