%{
#include "parser.h"
#include <string>
%}

digit [0-9]
letter [a-zA-Z]

%option outfile="lexer.cpp"  header-file="lexer.h"
%option warn nodefault
%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

%%

"{" { return STRUCT_BEGIN; }
"}" { return STRUCT_END; }
"[" { return ARRAY_BEGIN; }
"]" { return ARRAY_END; }
"," { return SEP; }
";" { return SEP; }
"=" { return ASSIGN; }
":" { return ASSIGN; }
":=" { return ASSIGN; }
{letter}({letter}|{digit})* {yylval->ident_value = new std::string(yytext); return IDENT;}
{digit}+{letter}? { yylval->ident_value = new std::string(yytext); return IDENT;}
[ \t] /* skip whitespace */;
"\n" { ++yyextra->line; }
. { printf("Unknown character [%c]\n",yytext[0]); return UNKNOWN; }

%%

// final code