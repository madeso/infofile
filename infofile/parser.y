%{
#define YY_NO_UNISTD_H
#include <cstdio>
#include <iostream>
#include <string>

#include "infofile/infofile.h"
#include "parser.h"
#include "lexer.h"
#include <sstream>

void yyerror(ParserData* expression, yyscan_t scanner, const char *error);
%}

%code requires {
	#ifndef YY_TYPEDEF_YY_SCANNER_T
		#define YY_TYPEDEF_YY_SCANNER_T
		typedef void* yyscan_t;
	#endif

	#include <string>
	#include <vector>

	#include "infofile/infofile.h"
	#ifndef PARSER_DATA_DEFINED
		#define PARSER_DATA_DEFINED
		struct ParserData {
			::infofile::Value* result;
			std::string file;
			unsigned int line;
			unsigned int ch;
			unsigned int pch;

			std::vector<std::string> errors;
		};
		void AdvanceCharacter(ParserData* data, unsigned int steps);
		void ResetCharacter(ParserData* data);
		void ReportError(ParserData* data, const std::string& msg);
	#endif
	#define YY_EXTRA_TYPE ParserData*
}

%union {
    ::std::string* ident_value;
    ::infofile::Node* node;
	::infofile::Value* value;
}

%output  "parser.cpp"
%defines "parser.h"

%define api.pure
%error-verbose
%pure-parser

%lex-param   { yyscan_t scanner }
%parse-param { ParserData* expression }
%parse-param { yyscan_t scanner }

%token STRUCT_BEGIN
%token STRUCT_END
%token ARRAY_BEGIN
%token ARRAY_END
%token SEP
%token ASSIGN
%token <ident_value> IDENT
%token UNKNOWN

%type <value> children struct_list array_value_list array_children_list
%type <node> pair

%start input

%%

input
	: children { expression->result = $1; }
	;
	
struct_list
	: pair
	{
		$$->children.push_back($1);
	}
	| struct_list pair
	{
		::infofile::NodePtr n($2);
		$1->children.push_back(n);
		$$ = $1;
	}
	;

array_value_list
	: IDENT
	{
		$$->children.push_back( new ::infofile::Node("", *$1 ) );
		delete $1;
	}
	| array_value_list IDENT
	{
		$1->children.push_back( new ::infofile::Node("", *$2 ) );
		delete $2;
		$$ = $1;
	}
	;

array_children_list
	: children
	| array_children_list children
	;

children
	: STRUCT_BEGIN struct_list STRUCT_END { $$ = $2; }
	| ARRAY_BEGIN array_value_list ARRAY_END  { $$ = $2; }
	| ARRAY_BEGIN array_children_list ARRAY_END  { $$ = $2; }
	;

pair
	: IDENT[K] ASSIGN IDENT[V] SEP
	{
		$$ = new ::infofile::Node(*$K, *$V);
		delete $K;
		delete $V;
	}
	| IDENT children SEP
	{
		::infofile::Node* n = new ::infofile::Node(*$1);
		n->children = $2;
		delete $1;
		$$ = n;
	}
	;

%%

void yyerror(ParserData* expression, yyscan_t scanner, const char *error) {
	ReportError(expression, error);
}

void ReportError(ParserData* data, const std::string& msg) {
	std::stringstream ss;
	ss << data->file << "(" << data->line << ":" << data->ch << "): " << msg << "\n";
	data->errors.push_back(ss.str());
}

// int yyparse(infofile::Value** expression, yyscan_t scanner);
 
//void ::infofile::Parse(const char *expr, infofile::Value* val)
void ::infofile::Parse(const String& filename, const String& data, ::infofile::Value* value, std::vector<std::string>* errors)
{
    ParserData expression;
	expression.line = 1;
	ResetCharacter(&expression);
	expression.file = filename;
    yyscan_t scanner;
    YY_BUFFER_STATE state;
 
    if (yylex_init(&scanner)) {
		errors->push_back("Couldn't initialize parser");
        return;
    }

	::yyset_extra (&expression, scanner);
 
    state = yy_scan_string(data.c_str(), scanner);
 
    if (yyparse(&expression, scanner)) {
        *errors = expression.errors;
		errors->push_back("Error parsing input");
        return;
    }
 
    yy_delete_buffer(state, scanner);
 
    yylex_destroy(scanner);
 
    *value = *expression.result;
	delete expression.result;
	*errors = expression.errors;
}

void AdvanceCharacter(ParserData* data, unsigned int steps)
{
	data->ch += data-> pch;
	data->pch = steps;
}

void ResetCharacter(ParserData* data)
{
	data->ch=0;
	data->pch = 1;
}