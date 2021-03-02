#include "catch.hpp"
#include "catchy/stringeq.h"
#include "catchy/vectorequals.h"
#include "fmt/core.h"
#include "infofile/lexer.h"
#include "infofile/reader.h"

std::vector<infofile::Token> Tokenize(const std::string& str, std::vector<std::string>* errors)
{
    auto file = infofile::StringReader("inline", str);
    auto lexer = infofile::Lexer(&file, errors);

    std::vector<infofile::Token> r;
    while (lexer.Peek().type != infofile::TokenType::ENDOFFILE)
    {
        r.emplace_back(lexer.Read());
    }
    return r;
}

catchy::FalseString VectorEquals(const std::vector<infofile::Token>& lhs, const std::vector<infofile::Token>& rhs)
{
    return catchy::VectorEquals(
        lhs,
        rhs,
        [](const infofile::Token& f) -> std::string { return fmt::format("({} {})", f.type, f.value); },
        [](const infofile::Token& a, const infofile::Token& b) -> catchy::FalseString {
            if (a.type == b.type)
            {
                if (a.type == infofile::TokenType::IDENT)
                {
                    if (a.value != b.value)
                    {
                        return catchy::FalseString::False(fmt::format("Value differs: {} vs {}", a.value, b.value));
                    }
                }
                return catchy::FalseString::True();
            }
            else
            {
                return catchy::FalseString::False(fmt::format("Type and/or value differs: {}{} vs {}{}", a.type, a.value, b.type, b.value));
            }
        });
}

using namespace infofile;

TEST_CASE("lexer tests", "[lexer]")
{
    SECTION("empty")
    {
        std::vector<std::string> errors;
        CHECK(VectorEquals(Tokenize("", &errors), {}));
        REQUIRE(catchy::StringEq(errors, {}));
    }

    SECTION("identifiers")
    {
        std::vector<std::string> errors;
        CHECK(VectorEquals(Tokenize("cat dog", &errors), {{TokenType::IDENT, "cat"}, {TokenType::IDENT, "dog"}}));
        REQUIRE(catchy::StringEq(errors, {}));
    }

    SECTION("identifiers with sep")
    {
        std::vector<std::string> errors;
        CHECK(VectorEquals(Tokenize("cat,dog", &errors), {{TokenType::IDENT, "cat"}, {TokenType::SEP, ""}, {TokenType::IDENT, "dog"}}));
        REQUIRE(catchy::StringEq(errors, {}));
    }
}