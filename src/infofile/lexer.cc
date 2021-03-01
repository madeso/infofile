#include "infofile/lexer.h"

#include <sstream>

#include "fmt/core.h"
#include "infofile/file.h"

namespace infofile
{
    Token::Token(TokenType t, const std::string& v)
        : type(t)
        , value(v)
    {
    }

    Lexer::Lexer(File* f, std::vector<std::string>* e)
        : file(f)
        , errors(e)
    {
    }

    namespace
    {
        bool IsWhitespace(char c)
        {
            switch (c)
            {
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                return true;
            default:
                return false;
            }
        }

        bool IsIdentChar(char c, bool first)
        {
            if (first)
            {
                const auto lower = 'a' <= c && c <= 'z';
                const auto upper = 'A' <= c && c <= 'Z';
                return lower || upper || c == '_';
            }
            else
            {
                const auto number = '0' <= c && c <= '9';
                const auto special = c == '.' || c == '@';
                return IsIdentChar(c, true) || number || special;
            }
        }
    }

    void Lexer::SkipWhitespace()
    {
        while (IsWhitespace(file->Peek()))
        {
            file->Read();
        }
    }

    Token Lexer::ReadIdent()
    {
        std::ostringstream ss;

        ss << file->Read();

        while (IsIdentChar(file->Peek(), false))
        {
            ss << file->Read();
        }

        return {TokenType::IDENT, ss.str()};
    }

    Token Lexer::DoRead()
    {
        SkipWhitespace();
        const auto c = file->Peek();
        switch (c)
        {
        case 0:
            return {TokenType::ENDOFFILE, "<EOF>"};
        case '{':
            file->Read();
            return {TokenType::STRUCT_BEGIN, "{"};
        case '}':
            file->Read();
            return {TokenType::STRUCT_END, "}"};
        case '[':
            file->Read();
            return {TokenType::ARRAY_BEGIN, "["};
        case ']':
            file->Read();
            return {TokenType::ARRAY_END, "]"};
        case ',':
            file->Read();
            return {TokenType::SEP, ","};
        case ';':
            file->Read();
            return {TokenType::SEP, ";"};
        case '=':
            file->Read();
            return {TokenType::ASSIGN, "="};
        case '+':
            file->Read();
            return {TokenType::COMBINE, "+"};
        case '\\':
            file->Read();
            return {TokenType::COMBINE, "\\"};
        case ':':
            file->Read();
            if (file->Peek() == '=')
            {
                file->Read();
                return {TokenType::ASSIGN, ":="};
            }
            else
            {
                return {TokenType::ASSIGN, ":"};
            }
        default:
            if (IsIdentChar(c, true))
            {
                return ReadIdent();
            }
            else
            {
                ReportError(fmt::format("Unknown character {}", c));
                file->Read();
                return {TokenType::UNKNOWN, fmt::format("{}", c)};
            }
        }
    }

    void Lexer::ReportError(const std::string& error)
    {
        errors->emplace_back(error);
    }

    Token Lexer::Read()
    {
        if (next)
        {
            auto r = *next;
            next = std::nullopt;
            return r;
        }
        else
        {
            return DoRead();
        }
    }

    Token Lexer::Peek()
    {
        if (next)
        {
            return *next;
        }
        else
        {
            next = DoRead();
            return *next;
        }
    }
}