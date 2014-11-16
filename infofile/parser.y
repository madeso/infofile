%{
#include <cstdio>
#include <iostream>
#include <string>

#include "infofile/infofile.h"
#include "parser.h"
#include "lexer.h"

void yyerror(const char *s);
%}

%code requires {
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif
}

%union {
    std::string* ident_value;
    infofile::Node* node;
	infofile::Value* value;
}

%output  "parser.cpp"
%defines "parser.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { infofile::Value** expression }
%parse-param { yyscan_t scanner }

%token BEGIN_STRUCT
%token END_STRUCT

%token STRUCT_BEGIN
%token STRUCT_END
%token ARRAY_BEGIN
%token ARRAY_END
%token SEP
%token ASSIGN
%token <ident_value> IDENT
%token UNKNOWN

%type <value> children;

%%

input
	: children { *expression = $1; }
	;
	
struct_list
	: pair { $$ = $1; }
	| struct_list pair { ATTACH($$, $1, $2); }
	|
	;

array_value_list
	: IDENT
	| array_value_list IDENT
	;

array_children_list
	: children
	| array_children_list children
	|
	;

children
	: BEGIN_STRUCT struct_list END_STRUCT
	| ARRAY_BEGIN array_value_list ARRAY_END
	| ARRAY_BEGIN array_children_list ARRAY_END
	;

pair
	: IDENT[K] ASSIGN IDENT[V] SEP {  }
	| IDENT children SEP {}
	;

%%

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	exit(-1);
}

int yyparse(infofile::Value** expression, yyscan_t scanner);
 
infofile::Value* Parse(const char *expr)
{
    infofile::Value* expression;
    yyscan_t scanner;
    YY_BUFFER_STATE state;
 
    if (yylex_init(&scanner)) {
        // couldn't initialize
        return NULL;
    }
 
    state = yy_scan_string(expr, scanner);
 
    if (yyparse(&expression, scanner)) {
        // error parsing
        return NULL;
    }
 
    yy_delete_buffer(state, scanner);
 
    yylex_destroy(scanner);
 
    return expression;
}
