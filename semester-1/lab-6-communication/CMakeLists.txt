cmake_minimum_required(VERSION 4.2)
project(lab_6_communication C)

set(CMAKE_C_STANDARD 11)

add_executable(lab_6_communication main.c
        1/sinchr/reader.c
        1/sinchr/writer.c
        1/unsinch/reader.c
        1/unsinch/writer.c
        2/named/reader.c
        2/named/writer.c
        2/unnamed/pipe.c)
