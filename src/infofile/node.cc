#include "infofile/node.h"

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
}
