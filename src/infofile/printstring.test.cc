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

    SECTION("escapes")
    {
        SECTION("quote")
        {
            REQUIRE(catchy::StringEq(PrintString("\""), "\"\\\"\""));
        }
        SECTION("newline")
        {
            REQUIRE(catchy::StringEq(PrintString("\n"), "\"\\n\""));
        }
        SECTION("linefeed")
        {
            REQUIRE(catchy::StringEq(PrintString("\r"), "\"\\r\""));
        }
        SECTION("tab")
        {
            REQUIRE(catchy::StringEq(PrintString("\t"), "\"\\t\""));
        }
        SECTION("null")
        {
            REQUIRE(catchy::StringEq(PrintString(std::string(1, '\0')), "\"\\0\""));
        }
        SECTION("backslash")
        {
            REQUIRE(catchy::StringEq(PrintString("\\"), "\"\\\\\""));
        }
    }

    SECTION("email")
    {
        REQUIRE(catchy::StringEq(PrintString("hello@hotmale.com"), "hello@hotmale.com"));
    }
}
