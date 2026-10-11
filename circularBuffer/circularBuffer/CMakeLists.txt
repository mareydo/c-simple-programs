add_library(circular_buffer_lib STATIC
    src/circularBuffer.cpp
)

target_include_directories(circular_buffer_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

target_compile_features(circular_buffer_lib PUBLIC cxx_std_20)
target_compile_options(circular_buffer_lib PRIVATE -Wall -Wextra -Wpedantic)

target_compile_definitions(circular_buffer_lib PRIVATE
    CIRCULAR_BUFFER_INTERNAL=1
    $<$<CONFIG:Debug>:CB_DEBUG=1>
)

if(CIRCULAR_BUFFER_LIB_WERROR)
    target_compile_options(circular_buffer_lib PRIVATE -Werror)
endif()

find_package(Threads REQUIRED)
target_link_libraries(circular_buffer_lib PRIVATE Threads::Threads)

add_library(circular_buffer_constants INTERFACE)
target_include_directories(circular_buffer_constants INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

include(GNUInstallDirs)

install(TARGETS circular_buffer_lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS circular_buffer_lib
    EXPORT circular_buffer_lib-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT circular_buffer_lib-targets
    FILE circular_buffer_libTargets.cmake
    NAMESPACE CircularBuffer::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/circular_buffer_lib
)