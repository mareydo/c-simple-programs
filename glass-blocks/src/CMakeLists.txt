cmake_minimum_required(VERSION 3.16)

project(project_name LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(project_core
    src/componentA/componentA.cpp
    src/componentB/componentB.cpp
)

target_include_directories(project_core PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(project_name
    src/main.cpp
)

target_link_libraries(project_name PRIVATE project_core)