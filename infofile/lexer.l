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

"{" { ++yyextra->ch; return STRUCT_BEGIN; }
"}" { ++yyextra->ch; return STRUCT_END; }
"[" { ++yyextra->ch; return ARRAY_BEGIN; }
"]" { ++yyextra->ch; return ARRAY_END; }
"," { ++yyextra->ch; return SEP; }
";" { ++yyextra->ch; return SEP; }
"=" { ++yyextra->ch; return ASSIGN; }
":" { ++yyextra->ch; return ASSIGN; }
":=" { yyextra->ch+=2; return ASSIGN; }
{letter}({letter}|{digit})* {yylval->ident_value = new std::string(yytext); yyextra->ch+=strlen(yytext); return IDENT;}
{digit}+{letter}? { yylval->ident_value = new std::string(yytext); yyextra->ch+=strlen(yytext); return IDENT;}
[ \t] { ++yyextra->ch; };
"\n" { ++yyextra->line; yyextra->ch=1; }
. { ++yyextra->ch; printf("Unknown character [%c]\n",yytext[0]); return UNKNOWN; }

%%

// final code