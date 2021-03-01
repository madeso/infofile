#ifndef INFOFILE_INFOFILE_H
#define INFOFILE_INFOFILE_H

#include <memory>
#include <string>
#include <vector>

namespace infofile
{
    /** A Node in the info file.
    */
    struct Node
    {
        Node();
        explicit Node(const std::string& name);
        Node(const std::string& name, const std::string& value);

        std::string name;
        std::string value;
        std::vector<std::shared_ptr<Node>> children;
    };

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

#endif  // INFOFILE_INFOFILE_H
