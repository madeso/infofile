#ifndef INFOFILE_INFOFILE_H
#define INFOFILE_INFOFILE_H

#include <memory>
#include <string>
#include <vector>

namespace infofile
{
    /** A Node in the info file.
    */
    class Node
    {
    public:
        Node();
        explicit Node(const std::string& name);
        Node(const std::string& name, const std::string& value);
        ~Node();

        const std::string& name() const;
        void set_name(const std::string& name);

        const std::string& value() const;
        void set_value(const std::string& value);

        std::shared_ptr<Node> children;  // first child
        std::shared_ptr<Node> next;      // next child

        std::shared_ptr<Node> GetFirstChild();
        void AddChild(std::shared_ptr<Node> child);
        void SetEndChild(std::shared_ptr<Node> child);
        unsigned int GetSibblingCount();
        unsigned int GetChildCount();

        void Clear();

    private:
        std::string name_;
        std::string value_;
    };

    class Printer
    {
    public:
        virtual ~Printer();
        virtual void Print(const std::string& str) = 0;
    };

    class PrintOptions
    {
    public:
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
