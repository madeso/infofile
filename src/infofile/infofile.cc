#include "infofile/infofile.h"

#include <cassert>
#include <iostream>
#include <sstream>

namespace infofile
{
    Node::Node()
        : children(nullptr)
        , next(nullptr)
    {
        assert(this);
    }

    Node::Node(const std::string& name)
        : name_(name)
        , value_("")
        , children(nullptr)
        , next(nullptr)
    {
        assert(this);
    }

    Node::Node(const std::string& name, const std::string& value)
        : name_(name)
        , value_(value)
        , children(nullptr)
        , next(nullptr)
    {
        assert(this);
    }

    Node::~Node()
    {
        assert(this);
        Clear();
    }

    const std::string& Node::name() const
    {
        assert(this);
        return name_;
    }

    void Node::set_name(const std::string& name)
    {
        assert(this);
        name_ = name;
    }

    const std::string& Node::value() const
    {
        assert(this);
        return value_;
    }

    void Node::set_value(const std::string& value)
    {
        assert(this);
        value_ = value;
    }

    Node* Node::GetFirstChild()
    {
        assert(this);
        if (children == nullptr)
            return nullptr;
        return children;
    }

    void Node::AddChild(Node* child)
    {
        assert(this);
        assert(child);
        if (children == nullptr)
            children = child;
        else
            children->SetEndChild(child);
    }

    void Node::SetEndChild(Node* child)
    {
        assert(this);
        assert(child);
        Node* self = this;
        Node* n = self->next;
        while (true)
        {
            if (n == nullptr)
            {
                self->next = child;
                return;
            }
            self = n;
            n = self->next;
        }
    }

    unsigned int Node::GetSibblingCount()
    {
        assert(this);
        unsigned int count = 1;  // start at 1 sincew we include this in the calculation
        Node* n = next;
        while (n)
        {
            ++count;
            n = n->next;
        }

        return count;
    }

    unsigned int Node::GetChildCount()
    {
        if (children == nullptr)
            return 0;
        else
            return children->GetSibblingCount();
    }

    void Node::Clear()
    {
        assert(this);
        if (children)
        {
            delete children;
            children = nullptr;
        }
        while (next)
        {
            Node* t = next->next;
            ;
            delete next;
            next = t;
        }
        next = nullptr;
        set_name("");
        set_value("");
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

    void PrintNode(Printer* printer, int indent, const PrintOptions& po, Node* node)
    {
        std::string tab;
        for (int i = 0; i < indent; ++i)
        {
            tab += po.tab;
        }
        std::stringstream ss;
        ss << tab;
        PrintString(ss, node->name());
        ss << " ";
        PrintString(ss, node->value());
        if (node->children)
        {
            ss << " {" << po.newline;
            printer->Print(ss.str());

            for (Node* c = node->children; c; c = c->next)
            {
                PrintNode(printer, indent + 1, po, c);
            }

            ss.str("");
            ss << tab << "}";
        }

        ss << po.term << po.newline;
        printer->Print(ss.str());
    }

    void Print(Printer* printer, const PrintOptions& po, Node* node)
    {
        for (Node* c = node; c; c = c->next)
        {
            PrintNode(printer, 0, po, c);
        }
    }

    class StdStringStreamPrinter : public Printer
    {
    public:
        std::stringstream ss;
        virtual void Print(const std::string& str)
        {
            ss << str;
        }
    };
    std::string PrintToString(const PrintOptions& po, Node* node)
    {
        StdStringStreamPrinter ss;
        Print(&ss, po, node);
        return ss.ss.str();
    }

    class CoutPrinter : public Printer
    {
    public:
        virtual void Print(const std::string& str)
        {
            std::cout << str;
        }
    };

    void PrintToConsole(const PrintOptions& po, Node* node)
    {
        CoutPrinter ss;
        Print(&ss, po, node);
    }

    Node* Parse(const std::string& filename, const std::string& data, std::vector<std::string>* errors)
    {
        // todo(Gustav): call parser
        return nullptr;
    }

    Node* ReadFile(const std::string& filename, std::vector<std::string>* errors)
    {
        // todo(Gustav): call parser
        return nullptr;
    }
}
