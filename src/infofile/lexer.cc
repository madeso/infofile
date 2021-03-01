#include "infofile/lexer.h"

#include <cassert>
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

    Token Lexer::ReadString(char type)
    {
        auto start = file->Read();
        assert(start == type);

        bool multiline = false;
        if (file->Peek() == type)
        {
            file->Read();
            if (file->Peek() == type)
            {
                file->Read();
                multiline = true;
            }
            else
            {
                return {TokenType::IDENT, ""};
            }
        }

        std::ostringstream ss;

        auto on_escape_char = [&]() {
            file->Read();
            switch (file->Peek())
            {
            case 'n':
                file->Read();
                ss << '\n';
                break;
            case 't':
                file->Read();
                ss << '\t';
                break;
            case '0':
                file->Read();
                ss << 0;
                break;
            case '"':
                file->Read();
                ss << '"';
                break;
            case '\'':
                file->Read();
                ss << '\'';
                break;
            default:
                ReportError(fmt::format("Invalid escape character {}", file->Peek()));
                ss << file->Read();
                break;
            }
        };

        while (file->Peek() != 0)
        {
            if (multiline)
            {
                if (file->Peek() == type)
                {
                    file->Read();
                    if (file->Peek() == type)
                    {
                        file->Read();

                        if (file->Peek() == type)
                        {
                            file->Read();
                            return {TokenType::IDENT, ss.str()};
                        }
                        else
                        {
                            ss << type << type;
                        }
                    }
                    else
                    {
                        ss << type;
                    }
                }

                switch (file->Peek())
                {
                case '\\':
                    on_escape_char();
                    break;
                default:
                    ss << file->Read();
                    break;
                }
            }
            else
            {
                if (file->Peek() == type)
                {
                    file->Read();
                    return {TokenType::IDENT, ss.str()};
                }

                switch (file->Peek())
                {
                case '\n':
                case '\r':
                case '\t':
                    file->Read();
                    ReportError("Invalid whitespace in string!");
                    return {TokenType::IDENT, ss.str()};
                case '\\':
                    on_escape_char();
                    break;
                default:
                    ss << file->Read();
                    break;
                }
            }
        }

        ReportError(fmt::format("Missing {} at end of string", type));
        return {TokenType::IDENT, ss.str()};
    }

    Token Lexer::ReadVerbatimString(char type)
    {
        auto start = file->Read();
        assert(start == type);

        std::ostringstream ss;

        while (file->Peek() != 0)
        {
            if (file->Peek() == type)
            {
                file->Read();
                if (file->Peek() == type)
                {
                    ss << file->Read();
                }
                else
                {
                    return {TokenType::IDENT, ss.str()};
                }
            }

            switch (file->Peek())
            {
            case '\n':
            case '\r':
            case '\t':
                file->Read();
                ReportError("Invalid whitespace in string!");
                return {TokenType::IDENT, ss.str()};
            default:
                ss << file->Read();
                break;
            }
        }

        ReportError(fmt::format("Missing {} at end of verbatim string", type));
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
        case '"':
            return ReadString('"');
        case '\'':
            return ReadString('\'');
        case '@':
            file->Read();
            switch (file->Peek())
            {
            case '\'':
                return ReadVerbatimString('\'');
            case '"':
                return ReadVerbatimString('"');
            default:
                ReportError(fmt::format("Invalid character followed by verbatinm string marker @ {}", file->Peek()));
                file->Read();
                break;
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
        errors->emplace_back(fmt::format("{}({}:{}): {}", file->filename, file->line + 1, file->offset + 1, error));
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