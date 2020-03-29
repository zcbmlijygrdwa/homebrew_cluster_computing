cmake_minimum_required(VERSION 3.4.1)

project(homebrew_cluster_computing)

include_directories(
include
)

add_executable(example_master 
src/example_master.cpp
src/master.cpp
)
target_link_libraries(example_master
)

add_executable(example_slave 
src/example_slave.cpp
src/slave.cpp
)
target_link_libraries(example_slave
)

add_library(${PROJECT_NAME}_lib
src/master.cpp
src/slave.cpp
)

target_include_directories(${PROJECT_NAME}_lib PUBLIC include)
