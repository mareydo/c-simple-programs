cmake_minimum_required(VERSION 3.16)

set(project_name "circular_buffer_project")

project(${project_name} VERSION 0.1 LANGUAGES CXX)

add_subdirectory(circularBuffer)
add_subdirectory(app)

option(CIRCULAR_BUFFER_LIB_WERROR "Warnings as errors" OFF)
