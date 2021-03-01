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
        void Unput(char c);

        char Count(char c);

        std::string filename;
        int line;
        int offset;
        std::optional<char> next;
    };
}