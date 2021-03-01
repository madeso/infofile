#include "infofile/infofile.h"

#include <cassert>
#include <iostream>
#include <sstream>

namespace infofile
{
    Node::Node()
    {
    }

    Node::Node(const std::string& name)
        : name(name)
        , value("")
    {
    }

    Node::Node(const std::string& name, const std::string& value)
        : name(name)
        , value(value)
    {
    }

    Printer::~Printer()
    {
    }

    PrintOptions::PrintOptions()
        : tab("\t")
        , newline("\n")
        , term(";")
    {
    }

    void PrintString(std::stringstream& ss, const std::string& str)
    {
        std::string::size_type index = str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
        if (index == -1 && str.empty() == false)
        {
            ss << str;
        }
        else
        {
            ss << "\"" << str << "\"";
        }
    }

    void PrintNode(Printer* printer, int indent, const PrintOptions& po, std::shared_ptr<Node> node)
    {
        std::string tab;
        for (int i = 0; i < indent; ++i)
        {
            tab += po.tab;
        }
        std::stringstream ss;
        ss << tab;
        PrintString(ss, node->name);
        ss << " ";
        PrintString(ss, node->value);
        if (node->children.empty() == false)
        {
            ss << " {" << po.newline;
            printer->Print(ss.str());

            for (std::shared_ptr<Node> c : node->children)
            {
                PrintNode(printer, indent + 1, po, c);
            }

            ss.str("");
            ss << tab << "}";
        }

        ss << po.term << po.newline;
        printer->Print(ss.str());
    }

    void Print(Printer* printer, const PrintOptions& po, std::shared_ptr<Node> node)
    {
        PrintNode(printer, 0, po, node);
    }

    struct StdStringStreamPrinter : public Printer
    {
        std::stringstream ss;
        virtual void Print(const std::string& str)
        {
            ss << str;
        }
    };

    std::string PrintToString(const PrintOptions& po, std::shared_ptr<Node> node)
    {
        StdStringStreamPrinter ss;
        Print(&ss, po, node);
        return ss.ss.str();
    }

    struct CoutPrinter : public Printer
    {
        virtual void Print(const std::string& str)
        {
            std::cout << str;
        }
    };

    void PrintToConsole(const PrintOptions& po, std::shared_ptr<Node> node)
    {
        CoutPrinter ss;
        Print(&ss, po, node);
    }

    std::shared_ptr<Node> Parse(const std::string& filename, const std::string& data, std::vector<std::string>* errors)
    {
        // todo(Gustav): call parser
        return nullptr;
    }

    std::shared_ptr<Node> ReadFile(const std::string& filename, std::vector<std::string>* errors)
    {
        // todo(Gustav): call parser
        return nullptr;
    }
}
