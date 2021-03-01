#pragma once

#include <memory>
#include <string>
#include <vector>

#include "infofile/node.h"

namespace infofile
{
    struct Printer
    {
        virtual ~Printer();
        virtual void Print(const std::string& str) = 0;
    };

    struct PrintOptions
    {
        PrintOptions();
        std::string tab;
        std::string newline;
        std::string term;
    };

    void Print(Printer* printer, const PrintOptions& po, std::shared_ptr<Node> node);
    std::string PrintToString(const PrintOptions& po, std::shared_ptr<Node> node);
    void PrintToConsole(const PrintOptions& po, std::shared_ptr<Node> node);

    std::shared_ptr<Node> Parse(const std::string& filename, const std::string& data, std::vector<std::string>* errors);
    std::shared_ptr<Node> ReadFile(const std::string& filename, std::vector<std::string>* errors);

}
