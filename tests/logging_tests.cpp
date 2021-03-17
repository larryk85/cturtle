#include <iostream>
#include <string>

#include <bluegrass/cturtle.hpp>

#include <catch2/catch.hpp>

using namespace bluegrass;
using namespace bluegrass::cturtle;

TEST_CASE("Testing logging", "[logging_tests]") {
   info_log("hello");
}
