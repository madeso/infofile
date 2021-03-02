#include "infofile/chars.h"

namespace infofile
{
    bool IsNumber(char c)
    {
        return '0' <= c && c <= '9';
    }

    bool IsAlpha(char c)
    {
        const auto lower = 'a' <= c && c <= 'z';
        const auto upper = 'A' <= c && c <= 'Z';
        return upper || lower;
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
            const auto number = IsNumber(c);
            const auto special = c == '.' || c == '@';
            return IsIdentChar(c, true) || number || special;
        }
    }

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

    bool IsHex(char c)
    {
        const auto number = IsNumber(c);
        const auto lower = 'a' <= c && c <= 'f';
        const auto upper = 'A' <= c && c <= 'F';
        return number || lower || upper;
    }

    bool IsBinary(char c)
    {
        return c == '0' || c == '1';
    }
}