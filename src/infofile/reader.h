#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "infofile/file.h"

namespace infofile
{
    struct FileReader : public File
    {
        std::ifstream stream;

        FileReader(const std::string& filename);

        char DoRead() override;
    };

    struct StringReader : public File
    {
        std::istringstream stream;

        StringReader(const std::string& filename, const std::string& data);

        char DoRead() override;
    };
}