#include "infofile/parser.h"

#include <cassert>
#include <sstream>

#include "fmt/format.h"
#include "infofile/lexer.h"
#include "infofile/node.h"
#include "infofile/printstring.h"

namespace infofile
{
    namespace
    {
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
    }

    Parser::Parser(Lexer* l)
        : lexer(l)
    {
    }

    std::shared_ptr<Node> Parser::ReadRootNode()
    {
        auto first_token = lexer->Peek();
        auto node = std::make_shared<Node>();
        switch (first_token.type)
        {
        case TokenType::ARRAY_BEGIN:
            ParseArray(node);
            return node;
        case TokenType::STRUCT_BEGIN:
            ParseStruct(node);
            return node;
        default:
            ParseStructMembers(node);
            return node;
        }
    }

    std::shared_ptr<Node> Parser::ReadNode()
    {
        const auto key_token = lexer->Peek();
        const auto has_key = key_token.type == TokenType::IDENT;
        const auto key = has_key ? ReadIdent() : "";
        std::string value;
        if (has_key)
        {
            const auto has_assign = lexer->Peek().type == TokenType::ASSIGN;
            if (has_assign)
            {
                lexer->Read();
            }
            const auto has_value = lexer->Peek().type == TokenType::IDENT;

            value = has_value ? ReadIdent() : "";

            if (has_value && lexer->Peek().type == TokenType::ASSIGN)
            {
                lexer->Read();
            }
        }
        else
        {
            value = lexer->Peek().type == TokenType::IDENT ? ReadIdent() : "";
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
        case TokenType::IDENT:
            return node;
        case TokenType::SEP:
            return node;
        case TokenType::ENDOFFILE:
            return node;
        default:
            lexer->ReportError(fmt::format("Invalid token {} in Node({} = {}), could either be [ or a {{", next.ValueForPrint(), PrintString(key), PrintString(value)));
        }
        return nullptr;
    }

    std::shared_ptr<Node> Parser::ReadValue()
    {
        const auto next = lexer->Peek();
        switch (next.type)
        {
        case TokenType::ARRAY_BEGIN:
        {
            auto node = std::make_shared<Node>("", "");
            ParseArray(node);
            return node;
        }
        case TokenType::STRUCT_BEGIN:
        {
            auto node = std::make_shared<Node>("", "");
            ParseStruct(node);
            return node;
        }
        case TokenType::IDENT:
            return std::make_shared<Node>("", ReadIdent());
        default:
            lexer->ReportError(fmt::format("Invalid token {} in array value, could either be [ or a {{", next.ValueForPrint()));
            return nullptr;
        }
    }

    std::string Parser::ReadIdent()
    {
        const auto read = lexer->Read();
        assert(read.type == TokenType::IDENT);

        std::ostringstream ret;
        ret << read.value;

        while (lexer->Peek().type == TokenType::COMBINE && lexer->Peek().type != TokenType::ENDOFFILE)
        {
            const auto combine = lexer->Read();
            assert(combine.type == TokenType::COMBINE);

            if (lexer->Peek().type != TokenType::IDENT)
            {
                lexer->ReportError(fmt::format("Expecting ident after {} but found {}", combine.value, lexer->Peek().ValueForPrint()));
                return ret.str();
            }

            const auto ident = lexer->Read();
            assert(ident.type == TokenType::IDENT);
            ret << ident.value;
        }

        return ret.str();
    }

    void Parser::ParseArray(std::shared_ptr<Node> root)
    {
        auto start = lexer->Read();
        assert(start.type == TokenType::ARRAY_BEGIN);

        while (!IsOneOf(lexer->Peek().type, {TokenType::ARRAY_END, TokenType::ENDOFFILE}))
        {
            auto node = ReadValue();
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

        if (lexer->Peek().type == TokenType::ARRAY_END)
        {
            lexer->Read();
        }
        else
        {
            lexer->ReportError(fmt::format("Expected ] but found {}", lexer->Peek().ValueForPrint()));
        }
    }

    void Parser::ParseStruct(std::shared_ptr<Node> root)
    {
        auto start = lexer->Read();
        assert(start.type == TokenType::STRUCT_BEGIN);

        ParseStructMembers(root);

        if (lexer->Peek().type == TokenType::STRUCT_END)
        {
            lexer->Read();
        }
        else
        {
            lexer->ReportError(fmt::format("Expected }} but found {}", lexer->Peek().ValueForPrint()));
        }
    }

    void Parser::ParseStructMembers(std::shared_ptr<Node> root)
    {
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
    }
}
