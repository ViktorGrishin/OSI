cmake_minimum_required(VERSION 4.2)
project(lab_4_vmemory C)

set(CMAKE_C_STANDARD 11)

add_executable(lab_4_vmemory main.c
        1/main.c
        2/base_check.c
        2/recursive.c
        2/protected_all.c
        2/protected_read.c)
