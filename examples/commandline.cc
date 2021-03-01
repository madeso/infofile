#include <iostream>

#include "infofile/infofile.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Missing argument\n";
        return 1;
    }

    std::vector<std::string> errors;
    infofile::Node* val = infofile::ReadFile(argv[1], &errors);
    for (std::vector<std::string>::iterator i = errors.begin(); i != errors.end(); ++i)
    {
        std::cerr << *i << "\n";
    }
    if (val != 0)
    {
        infofile::PrintToConsole(infofile::PrintOptions(), val);
        delete val;
    }

    return 0;
}
