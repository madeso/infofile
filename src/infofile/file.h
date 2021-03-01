#pragma once

#include <optional>
#include <string>

namespace infofile
{
    struct File
    {
        explicit File(const std::string& fn);
        virtual ~File() = default;

        virtual char DoRead() = 0;

        char Read();
        char Peek();

        std::string filename;
        std::optional<char> next;
    };
}