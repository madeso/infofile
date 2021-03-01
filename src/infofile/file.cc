#include "infofile/file.h"

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
        if (next)
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