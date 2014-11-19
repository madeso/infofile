%{
#include "parser.h"
#include <string>
#include <sstream>
#include <iostream>

#define DEBUGLEXER(x) std::cout << x << " "
%}

digit [0-9]
letter [a-zA-Z]

%option outfile="lexer.cpp"  header-file="lexer.h"
%option warn nodefault
%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

%%

"{" { AdvanceCharacter(yyextra, 1); DEBUGLEXER("SBEG"); return STRUCT_BEGIN; }
"}" { AdvanceCharacter(yyextra, 1); DEBUGLEXER("SEND");return STRUCT_END; }
"[" { AdvanceCharacter(yyextra, 1); DEBUGLEXER("ABEG");return ARRAY_BEGIN; }
"]" { AdvanceCharacter(yyextra, 1); DEBUGLEXER("AEND"); return ARRAY_END; }
"," { AdvanceCharacter(yyextra, 1); DEBUGLEXER("SEP"); return SEP; }
";" { AdvanceCharacter(yyextra, 1); DEBUGLEXER("SEP"); return SEP; }
"=" { AdvanceCharacter(yyextra, 1); DEBUGLEXER("ASS"); return ASSIGN; }
":" { AdvanceCharacter(yyextra, 1); DEBUGLEXER("ASS"); return ASSIGN; }
":=" { AdvanceCharacter(yyextra, 2); DEBUGLEXER("ASS"); return ASSIGN; }
{letter}({letter}|{digit})* {yylval->ident_value = new std::string(yytext); DEBUGLEXER("IDT-"<<yytext); AdvanceCharacter(yyextra, strlen(yytext)); return IDENT;}
{digit}+{letter}? { yylval->ident_value = new std::string(yytext); AdvanceCharacter(yyextra, yyextra->ch+=strlen(yytext)); DEBUGLEXER("NUM-"<<yytext); return IDENT;}
[ \t] { AdvanceCharacter(yyextra, 1); };
"\n" { ++yyextra->line; ResetCharacter(yyextra); }
. { AdvanceCharacter(yyextra, 1); std::stringstream ss; char c = yytext[0]; int i = c; ss << "Unknown character [" << c << "(" << i << ")]"; ReportError(yyextra, ss.str()); return UNKNOWN; }

%%

// final code