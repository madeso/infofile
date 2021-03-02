#include "infofile/node.h"

namespace infofile
{
    Node::Node()
    {
    }

    Node::Node(const std::string& n)
        : name(n)
        , value("")
    {
    }

    Node::Node(const std::string& n, const std::string& v)
        : name(n)
        , value(v)
    {
    }
}
