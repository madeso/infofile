#include "catch.hpp"
#include "catchy/stringeq.h"
#include "infofile/printstring.h"

using namespace infofile;

TEST_CASE("print string tests", "[printstring]")
{
    SECTION("empty")
    {
        REQUIRE(catchy::StringEq(PrintString(""), "\"\""));
    }

    SECTION("abc")
    {
        REQUIRE(catchy::StringEq(PrintString("abc"), "abc"));
    }

    SECTION("a b c")
    {
        REQUIRE(catchy::StringEq(PrintString("a b c"), "\"a b c\""));
    }
}
