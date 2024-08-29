cmake_minimum_required(VERSION 3.28)
project(project_elevator C)

set(CMAKE_C_STANDARD 99)

add_executable(project_elevator_single src/apps/single_elevator.c
)

add_executable(project_elevator_multi src/apps/multi_elevator.c
)
