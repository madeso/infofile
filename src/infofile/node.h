#pragma once

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
}