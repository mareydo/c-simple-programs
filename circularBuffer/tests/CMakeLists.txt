include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
)
set(CMAKE_CXX_CLANG_TIDY "")
FetchContent_MakeAvailable(Catch2)
# restore after
set(CMAKE_CXX_CLANG_TIDY 
    "clang-tidy;-checks=modernize-*,bugprone-*,-modernize-use-trailing-return-type;--header-filter=^${CMAKE_SOURCE_DIR}/(circularBuffer|app)/.*")
FetchContent_MakeAvailable(Catch2)

add_executable(circular_buffer_lib_tests test_circularBuffer.cpp)
target_link_libraries(circular_buffer_lib_tests PRIVATE circular_buffer_lib Catch2::Catch2WithMain)


include(CTest)
include(Catch)


catch_discover_tests(circular_buffer_lib_tests
    PROPERTIES
        LABELS "slow"
        TIMEOUT 3
)


# add_test(NAME fixture_setup
#     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/sensor_tmp)
# add_test(NAME fixture_cleanup
#     COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/sensor_tmp)
# add_test(NAME fixture_user
#     COMMAND ${CMAKE_COMMAND} -E echo "using sensor_tmp")

# set_tests_properties(fixture_setup   PROPERTIES FIXTURES_SETUP   sensor_fixture)
# set_tests_properties(fixture_cleanup PROPERTIES FIXTURES_CLEANUP  sensor_fixture)
# set_tests_properties(fixture_user    PROPERTIES FIXTURES_REQUIRED sensor_fixture)

add_test(NAME check_version
    COMMAND circular_buffer_d --version
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/app)

set_tests_properties(check_version PROPERTIES
    PASS_REGULAR_EXPRESSION "1\\.0"
)