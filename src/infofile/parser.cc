#include "infofile/parser.h"

#include <cassert>

#include "fmt/format.h"
#include "infofile/lexer.h"
#include "infofile/node.h"

namespace infofile
{
    Parser::Parser(Lexer* l)
        : lexer(l)
    {
    }

    std::shared_ptr<Node> Parser::ReadNode()
    {
        const auto key_token = lexer->Peek();
        const auto has_key = key_token.type == TokenType::IDENT;
        const auto key = has_key ? lexer->Read().value : "";
        std::string value;
        if (has_key)
        {
            const auto has_assign = lexer->Peek().type == TokenType::ASSIGN;
            if (has_assign)
            {
                lexer->Read();
            }
            const auto has_value = lexer->Peek().type == TokenType::IDENT;

            if (has_assign && !has_value)
            {
                lexer->ReportError(fmt::format("Expected value but found {}", lexer->Peek().value));
            }

            value = has_value ? lexer->Read().value : "";
        }
        else
        {
            value = lexer->Peek().type == TokenType::IDENT ? lexer->Read().value : "";
        }

        auto node = std::make_shared<Node>(key, value);

        const auto next = lexer->Peek();
        switch (next.type)
        {
        case TokenType::ARRAY_BEGIN:
            ParseArray(node);
            return node;
        case TokenType::STRUCT_BEGIN:
            ParseStruct(node);
            return node;
        case TokenType::STRUCT_END:
            return node;
        case TokenType::ARRAY_END:
            return node;
        case TokenType::IDENT:
            return node;
        case TokenType::SEP:
            return node;
        default:
            lexer->ReportError(fmt::format("Invalid token {} in Node({} = {}), could either be [ or a {{", next.value, key, value));
        }
        return nullptr;
    }

    void Parser::ParseArray(std::shared_ptr<Node> root)
    {
    }

    bool IsOneOf(TokenType self, std::vector<TokenType> tt)
    {
        for (const auto t : tt)
        {
            if (t == self)
            {
                return true;
            }
        }

        return false;
    }

    void Parser::ParseStruct(std::shared_ptr<Node> root)
    {
        auto start = lexer->Read();
        assert(start.type == TokenType::STRUCT_BEGIN);

        while (!IsOneOf(lexer->Peek().type, {TokenType::STRUCT_END, TokenType::ENDOFFILE}))
        {
            auto node = ReadNode();
            if (!node)
            {
                return;
            }

            root->children.emplace_back(node);

            if (lexer->Peek().type == TokenType::SEP)
            {
                lexer->Read();
            }
        }

        if (lexer->Peek().type == TokenType::STRUCT_END)
        {
            lexer->Read();
        }
        else
        {
            lexer->ReportError(fmt::format("Expected }} but found {}", lexer->Peek().value));
        }
    }
}
