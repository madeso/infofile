#include "infofile/reader.h"

namespace infofile
{
    namespace
    {
        template <typename Stream>
        char PleaseRead(Stream& stream)
        {
            if (stream.good() == false)
            {
                return 0;
            }
            char c;
            stream.get(c);
            if (stream.good())
            {
                return c;
            }
            else
            {
                return 0;
            }
        }
    }

    FileReader::FileReader(const std::string& filename)
        : File(filename)
        , stream(filename, std::ios::binary)
    {
    }

    char FileReader::DoRead()
    {
        return PleaseRead(stream);
    }

    StringReader::StringReader(const std::string& filename, const std::string& data)
        : File(filename)
        , stream(data)
    {
    }

    char StringReader::DoRead()
    {
        return PleaseRead(stream);
    }
}