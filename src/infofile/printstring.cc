#include "infofile/printstring.h"

#include <sstream>

#include "fmt/core.h"
#include "infofile/chars.h"

namespace infofile
{
    namespace
    {
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