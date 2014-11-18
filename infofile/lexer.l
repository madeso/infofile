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

"{" { AdvanceCharacter(yyextra, 1); return STRUCT_BEGIN; }
"}" { AdvanceCharacter(yyextra, 1); return STRUCT_END; }
"[" { AdvanceCharacter(yyextra, 1); return ARRAY_BEGIN; }
"]" { AdvanceCharacter(yyextra, 1); return ARRAY_END; }
"," { AdvanceCharacter(yyextra, 1); return SEP; }
";" { AdvanceCharacter(yyextra, 1); return SEP; }
"=" { AdvanceCharacter(yyextra, 1); return ASSIGN; }
":" { AdvanceCharacter(yyextra, 1); return ASSIGN; }
":=" { AdvanceCharacter(yyextra, 2); return ASSIGN; }
{letter}({letter}|{digit})* {yylval->ident_value = new std::string(yytext); AdvanceCharacter(yyextra, strlen(yytext)); return IDENT;}
{digit}+{letter}? { yylval->ident_value = new std::string(yytext); AdvanceCharacter(yyextra, yyextra->ch+=strlen(yytext)); return IDENT;}
[ \t] { AdvanceCharacter(yyextra, 1); };
"\n" { ++yyextra->line; ResetCharacter(yyextra); }
. { AdvanceCharacter(yyextra, 1); printf("Unknown character [%c]\n",yytext[0]); return UNKNOWN; }

%%

// final code