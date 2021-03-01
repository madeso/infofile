#ifndef INFOFILE_INFOFILE_H
#define INFOFILE_INFOFILE_H

#include <string>
#include <vector>

#include "configuration.h"

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

        Node* children;  // first child
        Node* next;      // next child

        Node* GetFirstChild();
        void AddChild(Node* child);
        void SetEndChild(Node* child);
        unsigned int GetSibblingCount();
        unsigned int GetChildCount();

        void Clear();

#if INFOFILE_USE_BASIC_MEMCHECK
        static int ActiveCount();
#endif

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

    void Print(Printer* printer, const PrintOptions& po, Node* node);
    std::string PrintToString(const PrintOptions& po, Node* node);
    void PrintToConsole(const PrintOptions& po, Node* node);
    Node* Parse(const std::string& filename, const std::string& data, std::vector<std::string>* errors);
    Node* ReadFile(const std::string& filename, std::vector<std::string>* errors);

}

#endif  // INFOFILE_INFOFILE_H
