cmake_minimum_required(VERSION 3.27.8)
project(project_elevator C)

set(CMAKE_C_STANDARD 99)

add_executable(project_elevator src/apps/elevator_app.c
        src/apps/elevator_app.c
        src/modules/script_reader.c
        src/headers/script_reader.h
)


