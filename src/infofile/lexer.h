#pragma once

#include <optional>
#include <string>
#include <vector>

namespace infofile
{
    struct File;

    enum class TokenType
    {
        STRUCT_BEGIN,
        STRUCT_END,
        ARRAY_BEGIN,
        ARRAY_END,
        SEP,
        ASSIGN,
        IDENT,  // has value
        COMBINE,
        UNKNOWN,
        ENDOFFILE
    };

    struct Token
    {
        TokenType type;
        std::string value;

        Token(TokenType t, const std::string& v);
    };

    struct Lexer
    {
        Lexer(File* f, std::vector<std::string>* e);

        void SkipWhitespace();
        Token ReadIdent();
        Token ReadString(char type);
        Token ReadVerbatimString(char type);
        Token ReadHereDoc();

        void EatLineComment();
        void EatMultilineComment();

        Token DoRead();
        void ReportError(const std::string& error);

        Token Read();
        Token Peek();

        File* file;
        std::vector<std::string>* errors;

        std::optional<Token> next;
    };
}