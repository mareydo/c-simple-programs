cmake_minimum_required(VERSION 3.16)

set(project_name "circular_buffer_project")

project(${project_name} VERSION 0.1 LANGUAGES CXX)

configure_file(
    cmake/version.h.in
    ${CMAKE_BINARY_DIR}/version.h
)


option(CIRCULAR_BUFFER_LIB_ASAN "ASan+UBSan build" OFF)
if(CIRCULAR_BUFFER_LIB_ASAN)
    message("ASAN UBSAN Build")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

option(CIRCULAR_BUFFER_LIB_TSAN "TSan build" OFF)
if(CIRCULAR_BUFFER_LIB_TSAN)
    message("TSAN Build")
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif()

option(CIRCULAR_BUFFER_LIB_COVERAGE "Coverage build" OFF)
if(CIRCULAR_BUFFER_LIB_COVERAGE)
    add_compile_options(--coverage -fno-inline)
    add_link_options(--coverage)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CIRCULAR_BUFFER_LIB_CLANG_TIDY "Enable clang-tidy" OFF)
if(CIRCULAR_BUFFER_LIB_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY
        "clang-tidy;-checks=modernize-*,bugprone-*,-modernize-use-trailing-return-type;--header-filter=^${CMAKE_SOURCE_DIR}/.*")
endif()

install(FILES cmake/circular_buffer_libConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/circular_buffer_lib
)




add_subdirectory(circularBuffer)
add_subdirectory(app)

option(CIRCULAR_BUFFER_LIB_WERROR "Warnings as errors" OFF)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS AND NOT CMAKE_CROSSCOMPILING)
    enable_testing()
    add_subdirectory(tests)
endif()




