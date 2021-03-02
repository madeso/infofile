#pragma once

#include <memory>
#include <string>
#include <vector>

namespace infofile
{
    struct Lexer;
    struct Node;

    struct Parser
    {
        explicit Parser(Lexer* l);

        std::shared_ptr<Node> ReadRootNode();
        std::shared_ptr<Node> ReadNode();
        std::shared_ptr<Node> ReadValue();

        std::string ReadIdent();

        void ParseArray(std::shared_ptr<Node> root);
        void ParseStruct(std::shared_ptr<Node> root);
        void ParseStructMembers(std::shared_ptr<Node> root);

        Lexer* lexer;
    };
}
