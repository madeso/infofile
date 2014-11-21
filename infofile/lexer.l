%{
#include "parser.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

typedef std::vector<std::string> VectorString;

struct UnescapedData {
	std::string str;
	VectorString errors;
};

UnescapedData UnEscapeString(const std::string& str);

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
\"(\\.|[^\\"])*\" {UnescapedData data = UnEscapeString(yytext); for(VectorString::const_iterator it=data.errors.begin(); it!=data.errors.end(); ++it){ReportError(yyextra, *it);} yylval->ident_value = new std::string(data.str); AdvanceCharacter(yyextra, strlen(yytext)); return IDENT;}
'(\\.|[^\\'])*' {UnescapedData data = UnEscapeString(yytext); for(VectorString::const_iterator it=data.errors.begin(); it!=data.errors.end(); ++it){ReportError(yyextra, *it);} yylval->ident_value = new std::string(data.str); AdvanceCharacter(yyextra, strlen(yytext)); return IDENT;}
{digit}+{letter}? { yylval->ident_value = new std::string(yytext); AdvanceCharacter(yyextra, yyextra->ch+=strlen(yytext)); return IDENT;}
[ \t] { AdvanceCharacter(yyextra, 1); };
"\n" { ++yyextra->line; ResetCharacter(yyextra); }
. { AdvanceCharacter(yyextra, 1); std::stringstream ss; char c = yytext[0]; int i = c; ss << "Unknown character [" << c << "(" << i << ")]"; ReportError(yyextra, ss.str()); return UNKNOWN; }

%%

// final code

UnescapedData UnEscapeString(const std::string& str) {
	const std::string s = str.substr(1, str.size() - 2);

	UnescapedData ret;

	std::string res;
	std::string::const_iterator it = s.begin();
	while (it != s.end())
	{
		char c = *it++;
		if (c == '\\' && it != s.end())
		{
			char nextc = *it++;
			switch (nextc) {
			case '\\': c = '\\'; break;
			case 'n': c = '\n'; break;
			case 't': c = '\t'; break;
			case '\"': c = '\"'; break;
				// all other escapes
			default:
				std::stringstream ss;
				int i = nextc;
				ss << "Unknown escape character [" << nextc << "(" << i << ")]";
				ret.errors.push_back(ss.str());
				continue;
			}
		}
		res += c;
	}

	ret.str = res;
	return ret;
}