%{
#define YY_NO_UNISTD_H
#include <cstdio>
#include <iostream>
#include <string>

#include "infofile/infofile.h"
#include "parser.h"
#include "lexer.h"

void yyerror(::infofile::Value** expression, yyscan_t scanner, const char *error);
%}

%code requires {
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

#include <string>
#include "infofile/infofile.h"
}

%union {
    ::std::string* ident_value;
    ::infofile::Node* node;
	::infofile::Value* value;
}

%output  "parser.cpp"
%defines "parser.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { ::infofile::Value** expression }
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
	: children { *expression = $1; }
	;
	
struct_list
	: pair
	{
		$$ = $1;
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
		n->children = $2->children;
		delete $1;
		delete $2;
		$$ = n;
	}
	;

%%

void yyerror(::infofile::Value** expression, yyscan_t scanner, const char *error) {
	::std::cout << "EEK, parse error!  Message: " << error << "\n";
}

int yyparse(infofile::Value** expression, yyscan_t scanner);
 
//void ::infofile::Parse(const char *expr, infofile::Value* val)
void ::infofile::Parse(const String& data, ::infofile::Value* value)
{
    infofile::Value* expression;
    yyscan_t scanner;
    YY_BUFFER_STATE state;
 
    if (yylex_init(&scanner)) {
        // couldn't initialize
        return;// NULL;
    }
 
    state = yy_scan_string(data.c_str(), scanner);
 
    if (yyparse(&expression, scanner)) {
        // error parsing
        return; // NULL;
    }
 
    yy_delete_buffer(state, scanner);
 
    yylex_destroy(scanner);
 
    *value = *expression;
	delete expression;
}
