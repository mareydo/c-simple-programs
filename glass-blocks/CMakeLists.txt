cmake_minimum_required(VERSION 3.16)

project(glass-blocks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(glass-blocks
    src/main.cpp
    src/camera/camera.cpp
    src/i2c/i2c.cpp
    src/mp3/mp3.cpp
    src/oled/oled.cpp
    src/oled/font.cpp
    src/serial/serial.cpp
)

target_include_directories(glass-blocks PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)


target_link_libraries(glass-blocks i2c)