#include "infofile/infofile.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "fmt/core.h"
#include "infofile/file.h"
#include "infofile/lexer.h"
#include "infofile/parser.h"
#include "infofile/reader.h"

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
        : tab("  ")
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

    std::shared_ptr<Node> ParseFromFile(File* file, std::vector<std::string>* errors)
    {
        auto lexer = Lexer(file, errors);
        auto parser = Parser(&lexer);
        auto parsed = parser.ReadRootNode();
        if (lexer.Peek().type != TokenType::ENDOFFILE)
        {
            lexer.ReportError(fmt::format("Expected EOF after node but found {} instead", lexer.Peek().value));
        }
        return parsed;
    }

    std::shared_ptr<Node> Parse(const std::string& filename, const std::string& data, std::vector<std::string>* errors)
    {
        auto reader = StringReader{filename, data};
        return ParseFromFile(&reader, errors);
    }

    std::shared_ptr<Node> ReadFile(const std::string& filename, std::vector<std::string>* errors)
    {
        auto reader = FileReader{filename};
        return ParseFromFile(&reader, errors);
    }
}
