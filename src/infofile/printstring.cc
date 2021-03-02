#include "infofile/printstring.h"

#include <sstream>

#include "fmt/core.h"

namespace infofile
{
    namespace
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

        bool IsIdent(const std::string& str)
        {
            if (str.empty())
            {
                return false;
            }

            bool first = true;

            for (char c : str)
            {
                if (IsIdentChar(c, first) == false)
                {
                    return false;
                }
                first = false;
            }

            return true;
        }

        std::string CharToCharInString(char c)
        {
            switch (c)
            {
            case '\0':
                return "\\0";
            case '\n':
                return "\\n";
            case '\r':
                return "\\r";
            case '"':
                return "\\\"";
            case '\t':
                return "\\t";
            case '\\':
                return "\\\\";
            default:
                return fmt::format("{}", c);
            }
        }
    }

    std::string PrintString(const std::string& str)
    {
        if (IsIdent(str))
        {
            return str;
        }
        else
        {
            std::ostringstream ss;
            ss << "\"";
            for (auto i = 0; i < str.size(); i += 1)
            {
                const char c = str[i];
                ss << CharToCharInString(c);
            }
            ss << "\"";
            return ss.str();
        }
    }
}