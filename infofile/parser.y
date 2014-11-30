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
			yyscan_t* scanner;
			::infofile::Value* result;
			std::string file;
			unsigned int line;
			unsigned int ch;
			unsigned int pch;
			unsigned int multiline_comment;

			std::vector<std::string> errors;
		};
		void AdvanceCharacter(ParserData* data, unsigned int steps);
		void ResetCharacter(ParserData* data);
		void ReportError(ParserData* data, const std::string& msg);
	#endif
	#define YY_EXTRA_TYPE ParserData*
}

	/* any variable might be NULL, if so delete input and let NULL flow through */
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
%token COMBINE
%token UNKNOWN

%type <value> children struct_list array_value_list array_children_list
%type <node> pair
%type <ident_value> idents

%start input

%%

input
	: children { expression->result = $1; }
	| struct_list { expression->result = $1; }
	;

idents
	: IDENT
	{
		$$ = $1;
	}
	| idents COMBINE IDENT
	{
		if( $1 && $3 ) {
			*$1 += *$3;
		}
		else {
			delete $1;
			$1 = NULL;
		}
		if( $3 ) {
			delete $3;
			$3 = NULL;
		}
		$$ = $1;
	}
	;
	
struct_list
	: pair
	{
		if( $1 ) {
			$$ = new infofile::Value();
			::infofile::NodePtr n($1);
			$$->children.push_back(n);
		}
		else {
			$$ = NULL;
		}
	}
	| struct_list pair
	{
		if( $1 && $2 ) {
			::infofile::NodePtr n($2);
			$1->children.push_back(n);
		}
		else {
			if( $2 ) delete $2;
		}
		$$ = $1;
	}
	;

array_value_list
	: idents
	{
		if( $1 ) {
			$$ = new infofile::Value();
			::infofile::NodePtr n(new ::infofile::Node("", *$1 ));
			$$->children.push_back(n);
			delete $1;
		}
		else {
			$$ = NULL;
		}
	}
	| array_value_list idents
	{
		if( $1 && $2 ) {
			::infofile::NodePtr n(new ::infofile::Node("", *$2 ));
			$1->children.push_back( n );
		}
		if( $2 ) {
			delete $2;
		}
		$$ = $1;
	}
	;

array_children_list
	: children
	{
		if( $1 ) {
			::infofile::NodePtr n(new ::infofile::Node("", ""));
			n->children= $1;
			$$ = new infofile::Value();
			$$->children.push_back( n );
		}
		else {
			$$ = NULL;
		}
	}
	| array_children_list children
	{
		if( $1 && $2 ) {
			::infofile::NodePtr n(new ::infofile::Node("", ""));
			n->children = $2;
			$1->children.push_back( n );
		}
		$$ = $1;
	}
	;

children
	: STRUCT_BEGIN struct_list STRUCT_END { $$ = $2; }
	| ARRAY_BEGIN array_value_list ARRAY_END  { $$ = $2; }
	| ARRAY_BEGIN array_children_list ARRAY_END  { $$ = $2; }
	;

optional_assign
	:
	| ASSIGN
	;

optional_sep
	:
	| SEP
	;
	

pair
	: idents[K] optional_assign idents[V] optional_sep
	{
		if( $K && $V) {
			$$ = new ::infofile::Node(*$K, *$V);
		}
		else {
			$$ = NULL;
		}
		if( $K ) delete $K;
		if( $V ) delete $V;
	}
	| idents[K] children[C] SEP
	{
		::infofile::Node* n = NULL;
		if( $K && $C ) {
			n = new ::infofile::Node(*$K);
			n->children = $C;
		}
		else {
			if( $C ) delete $C;
		}
		if( $K ) delete $K;
		$$ = n;
	}
	| idents[K] optional_assign idents[V] children[C] SEP
	{
		::infofile::Node* n = NULL;
		if( $K && $V && $C ) {
			n = new ::infofile::Node(*$K, *$V);
			n->children = $C;
		}
		else {
			if( $C ) delete $C;
		}
		if( $K ) delete $K;
		if( $V ) delete $V;
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
 
::infofile::Value* ::infofile::Parse(const String& filename, const String& data, std::vector<std::string>* errors)
{
    ParserData expression;
	expression.line = 1;
	ResetCharacter(&expression);
	expression.file = filename;
	expression.multiline_comment = 0;
    yyscan_t scanner;
	expression.scanner = &scanner;
    YY_BUFFER_STATE state;
 
    if (yylex_init(&scanner)) {
		errors->push_back("Couldn't initialize parser");
        return 0;
    }

	::yyset_extra (&expression, scanner);
 
    state = yy_scan_string(data.c_str(), scanner);
 
    if (yyparse(&expression, scanner)) {
        *errors = expression.errors;
		errors->push_back("Error parsing input");
        return 0;
    }
 
    yy_delete_buffer(state, scanner);
 
    yylex_destroy(scanner);
 
	*errors = expression.errors;

	return expression.result;
}

::infofile::Value* ::infofile::ReadFile(const String& filename, std::vector<std::string>* errors)
{
    ParserData expression;
	expression.line = 1;
	ResetCharacter(&expression);
	expression.file = filename;
	expression.multiline_comment = 0;
    yyscan_t scanner;
	expression.scanner = &scanner;
    // YY_BUFFER_STATE state;
 
    if (yylex_init(&scanner)) {
		errors->push_back("Couldn't initialize parser");
        return 0;
    }

	::yyset_extra (&expression, scanner);

	FILE* f = fopen(filename.c_str(), "r");
	if( !f ) {
		errors->push_back("Unable to open file.");
		return 0;
	}
 
    // YY_FLUSH_BUFFER;
	// if(yyStringBufferActive) yy_delete_buffer(string_buffer);
	yyrestart(f, scanner);
 
    if (yyparse(&expression, scanner)) {
        *errors = expression.errors;
		errors->push_back("Error parsing input");
		fclose(f);
        return 0;
    }
 
    // yy_delete_buffer(state, scanner);
	fclose(f);
 
    yylex_destroy(scanner);
 
	*errors = expression.errors;

	return expression.result;
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