cmake_minimum_required(VERSION 3.15)
project(Zhuque)

set(CMAKE_CXX_STANDARD 20)

add_executable(Zhuque main.cpp task/Executor.h task/Task.h)