cmake_minimum_required(VERSION 3.26)
project(interview_task1 C)

set(CMAKE_C_STANDARD 11)

add_executable(interview_task1 main.c
        flashdriver.h
        flashutil.h
        flashutil.c)
