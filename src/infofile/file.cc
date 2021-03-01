#include "infofile/file.h"

#include <cassert>

namespace infofile
{
    File::File(const std::string& fn)
        : filename(fn)
        , line(0)
        , offset(0)
    {
    }

    char File::Read()
    {
        if (next.has_value())
        {
            auto r = *next;
            next = std::nullopt;
            return Count(r);
        }
        else
        {
            return Count(DoRead());
        }
    }

    char File::Peek()
    {
        if (next.has_value())
        {
            return *next;
        }
        else
        {
            next = DoRead();
            return *next;
        }
    }

    void File::Unput(char c)
    {
        assert(next.has_value() == false);
        next = c;
    }

    char File::Count(char c)
    {
        if (c == '\n')
        {
            offset = 0;
            line += 1;
        }
        else
        {
            offset += 1;
        }

        return c;
    }
}