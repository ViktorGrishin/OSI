cmake_minimum_required(VERSION 4.2)
project(lab_5_processes C)

set(CMAKE_C_STANDARD 11)

add_executable(lab_5_processes main.c
        base.c
        zombie.c
        parent_zombie.c)
