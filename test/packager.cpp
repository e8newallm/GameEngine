#include <iostream>

#include "tools/packager/packager.h"

#include "Catch2/src/catch2/catch_all.hpp"

inline void conversionTest(long long value)
{
  REQUIRE(strToNum(numToStr(value)) == value);
}

TEST_CASE("Packager", "[packager]") {

  SECTION("Number to byte conversion") {
    conversionTest(0x1123456789ABCDEF);
    conversionTest(0);
    conversionTest(-1);
  }
}