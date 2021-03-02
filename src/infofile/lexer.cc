#include "infofile/lexer.h"

#include <cassert>
#include <sstream>

#include "fmt/core.h"
#include "infofile/chars.h"
#include "infofile/file.h"
#include "infofile/printstring.h"

namespace infofile
{
    Token::Token(TokenType t, const std::string& v)
        : type(t)
        , value(v)
    {
    }

    std::string Token::ValueForPrint() const
    {
        if (type == TokenType::IDENT)
        {
            return PrintString(value);
        }
        else
        {
            return value;
        }
    }

    Lexer::Lexer(File* f, std::vector<std::string>* e)
        : file(f)
        , errors(e)
    {
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
        [[maybe_unused]] auto start = file->Read();
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
                ss << '\0';
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
        [[maybe_unused]] auto start = file->Read();
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

    Token Lexer::ReadHereDoc()
    {
        [[maybe_unused]] char first = file->Read();
        assert(first == '<');
        if (file->Peek() != '<')
        {
            ReportError(fmt::format("Expected < but found {} at the start of a here doc", file->Peek()));
            return {TokenType::IDENT, ""};
        }
        file->Read();

        char c;
        std::string name;
        int nameindex = -1;
        std::string data;
        std::string potential;
        int mystate = 0;
        bool done = false;
        bool eof = false;
        while (done == false)
        {
            if (done == false)
            {
                c = file->Read();
            }
            if (c == 0)
            {
                done = true;
                eof = true;
            }
            if (done == false)
            {
                switch (mystate)
                {
                case 0:  // detect name
                    if (c == ' ' || c == '\n' || c == '\t')
                    {
                        if (name.length() <= 0)
                        {
                            ReportError("EOF name is empty");
                        }
                        mystate = 1;
                        file->Unput(c);
                    }
                    else
                    {
                        name += c;
                    }
                    break;
                case 1:  // name detected, ignore until newline
                    if (c == '\n')
                    {
                        mystate = 2;
                    }
                    else
                    {
                        // ignore
                    }
                    break;
                case 2:  // parse heredoc
                    if (nameindex == -1 && c == '\n')
                    {
                        nameindex = 0;
                    }
                    else if (nameindex >= 0 && name[static_cast<size_t>(nameindex)] == c)
                    {
                        potential += c;
                        ++nameindex;
                        // static cast here to avoid compiler warning
                        // this is probably safe as nameindex must be postive here
                        assert(nameindex >= 0);
                        if (static_cast<unsigned int>(nameindex) >= name.size())
                        {
                            // if the new index is beyond the string we have matched end string/name
                            // can can continue to the last part and ignoring the end characters
                            mystate = 3;
                        }
                    }
                    else
                    {
                        if (nameindex >= 0)
                        {
                            data += "\n";
                            data += potential;
                            potential = "";
                            nameindex = -1;
                        }
                        if (c == '\n')
                        {
                            nameindex = 0;
                        }
                        else
                        {
                            data += c;
                        }
                    }
                    break;
                case 3:
                    if (c == '\n')
                    {
                        mystate = 4;
                    }
                    else
                    {
                        // ignore
                    }
                    break;
                case 4:
                    assert(0 && "shouldn't be here");
                    break;
                }
            }

            if (mystate > 3)
            {
                done = true;
            }
        }
        if (mystate < 4)
        {
            if (eof)
            {
                ReportError("Found EOF before heredoc end");
            }
        }
        if (eof)
        {
            // return {TokenType::IDENT, ""};
            return {TokenType::IDENT, data};
        }
        else
        {
            return {TokenType::IDENT, data};
        }
    }

    Token Lexer::ReadZeroBasedNumber()
    {
        auto zero = file->Read();
        assert(zero == '0');

        std::ostringstream mem;
        mem << zero;
        bool read = false;

        switch (file->Peek())
        {
        case 'x':
            mem << file->Read();
            while (IsHex(file->Peek()))
            {
                read = true;
                mem << file->Read();
            }
            if (!read)
            {
                ReportError("Unexpected end in hexadecimal number");
            }
            return {TokenType::IDENT, mem.str()};
        case 'b':
            mem << file->Read();
            while (IsBinary(file->Peek()))
            {
                read = true;
                mem << file->Read();
            }
            if (!read)
            {
                ReportError("Unexpected end in hexadecimal number");
            }
            return {TokenType::IDENT, mem.str()};
        default:
            return ReadNumber(true);
        }
    }

    Token Lexer::ReadNumber(bool zero_start)
    {
        std::ostringstream mem;
        bool valid_number = false;

        if (zero_start)
        {
            mem << '0';
            valid_number = true;
        }
        else
        {
            switch (file->Peek())
            {
            case '-':
                mem << file->Read();
                break;
            }
        }

        while (IsNumber(file->Peek()))
        {
            valid_number = true;
            mem << file->Read();
        }

        if (valid_number == false)
        {
            ReportError("Invalid number, needs atleast one number");
            return {TokenType::IDENT, mem.str()};
        }

        if (file->Peek() != '.')
        {
            switch (file->Peek())
            {
            case 'f':
            case 'F':
                mem << file->Read();
                break;
            }
            return {TokenType::IDENT, mem.str()};
        }

        mem << file->Read();
        valid_number = false;

        while (IsNumber(file->Peek()))
        {
            valid_number = true;
            mem << file->Read();
        }

        if (valid_number == false)
        {
            ReportError("Invalid number, needs atleast one number after decimal place");
            return {TokenType::IDENT, mem.str()};
        }

        switch (file->Peek())
        {
        case 'f':
        case 'F':
            mem << file->Read();
            break;
        }
        return {TokenType::IDENT, mem.str()};
    }

    Token Lexer::ReadColor()
    {
        const auto hash = file->Read();
        assert(hash == '#');

        std::ostringstream mem;
        mem << hash;
        while (IsHex(file->Peek()))
        {
            mem << file->Read();
        }

        const auto str = mem.str();
        switch (str.size())
        {
        case 4:
        case 7:
            return {TokenType::IDENT, str};
        default:
            ReportError(fmt::format("Invalid color definition({}), needs to be eiter 3 or 6 hexes long", str));
            return {TokenType::IDENT, str};
        }
    }

    void Lexer::EatLineComment()
    {
        while (file->Peek() != '\n' && file->Peek() != 0)
        {
            file->Read();
        }
    }

    void Lexer::EatMultilineComment()
    {
        int inside = 0;
        while (file->Peek() != 0)
        {
            auto c = file->Read();
            if (c == '*' && file->Peek() == '/')
            {
                file->Read();
                if (inside == 0)
                {
                    return;
                }
                else
                {
                    inside -= 1;
                }
            }
            else if (c == '/' && file->Peek() == '*')
            {
                file->Read();
                inside += 1;
            }
        }
    }

    Token Lexer::DoRead()
    {
        SkipWhitespace();

        while (file->Peek() == '/')
        {
            file->Read();
            switch (file->Peek())
            {
            case '/':
                file->Read();
                EatLineComment();
                break;
            case '*':
                file->Read();
                EatMultilineComment();
                break;
            default:
                ReportError(fmt::format("Found rougue / followed by invalid {} when parsing comments", file->Peek()));
                break;
            }
            SkipWhitespace();
        }

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
        case '-':
            return ReadNumber(false);
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
        case '<':
            return ReadHereDoc();
        case '"':
            return ReadString('"');
        case '\'':
            return ReadString('\'');
        case '#':
            return ReadColor();
        case '@':
            file->Read();
            switch (file->Peek())
            {
            case '\'':
                return ReadVerbatimString('\'');
            case '"':
                return ReadVerbatimString('"');
            default:
                ReportError(fmt::format("Invalid character followed by verbatinm string marker @: {}", file->Peek()));
                return {TokenType::IDENT, fmt::format("{}", file->Read())};
            }
        case '0':
            return ReadZeroBasedNumber();
        default:
            if (IsIdentChar(c, true))
            {
                return ReadIdent();
            }
            else if (IsNumber(c))
            {
                return ReadNumber(false);
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