#include "infofile/printstring.h"

#include <sstream>

namespace infofile
{
    std::string PrintString(const std::string& str)
    {
        std::ostringstream ss;
        std::string::size_type index = str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
        if (index == -1 && str.empty() == false)
        {
            ss << str;
        }
        else
        {
            ss << "\"" << str << "\"";
        }
        return ss.str();
    }

}