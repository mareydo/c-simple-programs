#include <catch2/catch_test_macros.hpp>
#include "circularBuffer/circularBuffer.hpp"

TEST_CASE( "Push one number", "[push]" )
{
    CircularBuffer cb;
    REQUIRE( cb.push(1) == true );
    REQUIRE( cb.size() == 1);
}

TEST_CASE( "Push one Pop one", "[push;pop]" ) 
{
    CircularBuffer cb;
    uint8_t shouldBeOne = 0;
    REQUIRE( cb.push(1) == true );
    REQUIRE( cb.size() == 1);
    REQUIRE( cb.pop(&shouldBeOne) == true );
    REQUIRE( cb.size() == 0);
    REQUIRE( shouldBeOne == 1);
}
