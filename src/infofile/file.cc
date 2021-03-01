#include "infofile/file.h"

namespace infofile
{
    File::File(const std::string& fn)
        : filename(fn)
    {
    }

    char File::Read()
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
}