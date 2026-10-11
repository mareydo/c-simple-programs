add_executable(circular_buffer_d main.cpp)


find_package(Threads REQUIRED)

target_link_libraries(circular_buffer_d PRIVATE
    circular_buffer_lib
    circular_buffer_constants
    Threads::Threads)

install(TARGETS circular_buffer_d
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)