%{
#include "parser.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cassert>

typedef std::vector<std::string> VectorString;

struct UnescapedData {
	std::string str;
	VectorString errors;
};

UnescapedData UnEscapeString(const std::string& str);

std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace);

%}

number [0-9]

%option outfile="lexer.cpp"  header-file="lexer.h"
%option warn nodefault
%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

%x comment

%%

	/* multiline parsing */
"/""*"+ {
	BEGIN(comment);
	++yyextra->multiline_comment;
}
<comment>"/""*"+ {
	BEGIN(comment);
	++yyextra->multiline_comment;
}
<comment>[^*/]* {
	/* eat anything that's not a '*' */
}
<comment>"*"+[^*\/]* {
	/* eat up '*'s not followed by '/'s */
}
<comment>"*"+"/" {
	--yyextra->multiline_comment;
	if( yyextra->multiline_comment == 0 ) {
		BEGIN(INITIAL);
	}
}

"{" {
	AdvanceCharacter(yyextra, 1);
	return STRUCT_BEGIN;
}

"}" {
	AdvanceCharacter(yyextra, 1);
	return STRUCT_END;
}

"[" {
	AdvanceCharacter(yyextra, 1);
	return ARRAY_BEGIN;
}

"]" {
	AdvanceCharacter(yyextra, 1);
	return ARRAY_END;
}

"," {
	AdvanceCharacter(yyextra, 1);
	return SEP;
}

";" {
	AdvanceCharacter(yyextra, 1);
	return SEP;
}

"=" {
	AdvanceCharacter(yyextra, 1);
	return ASSIGN;
}

":" {
	AdvanceCharacter(yyextra, 1);
	return ASSIGN;
}

":=" {
	AdvanceCharacter(yyextra, 2);
	return ASSIGN;
}

"+" {
	AdvanceCharacter(yyextra, 1);
	return COMBINE;
}

"\\" {
	AdvanceCharacter(yyextra, 1);
	return COMBINE;
}

	/* single line comment, ignore everything until the newline */
\/\/[^\n]* {}

	/* standard ident */
[a-zA-Z_][a-zA-Z0-9.@_]* {
	yylval->ident_value = new std::string(yytext);
	AdvanceCharacter(yyextra, strlen(yytext));
	return IDENT;
}

	/* css color long */
#[0-9a-fA-F]{6} {
	yylval->ident_value = new std::string(yytext);
	AdvanceCharacter(yyextra, strlen(yytext));
	return IDENT;
}

	/* css color short */
#[0-9a-fA-F]{3} {
	yylval->ident_value = new std::string(yytext);
	AdvanceCharacter(yyextra, strlen(yytext));
	return IDENT;
}

	/* string regex based on code found here http://stackoverflow.com/a/2039820 */
\"(\\.|[^(\\"|\n)])*\" {
	UnescapedData data = UnEscapeString(yytext);
	for(VectorString::const_iterator it=data.errors.begin(); it!=data.errors.end(); ++it)
	{
		ReportError(yyextra, *it);
	}
	yylval->ident_value = new std::string(data.str);
	AdvanceCharacter(yyextra, strlen(yytext));
	return IDENT;
}

	/* c# verbatim string */
@\"(\"\"|[^("|\n)])*\" {
	std::string str = yytext;
	str = str.substr(2, str.size()-3);
	str = ReplaceString(str, "\"\"", "\"");
	yylval->ident_value = new std::string(str);
	AdvanceCharacter(yyextra, strlen(yytext));
	return IDENT;
}

	/* C# verbatim string, python string syntax */
@'(''|[^('|\n)])*' {
	std::string str = yytext;
	str = str.substr(2, str.size()-3);
	str = ReplaceString(str, "''", "'");
	yylval->ident_value = new std::string(str);
	AdvanceCharacter(yyextra, strlen(yytext));
	return IDENT;
}


	/* multiline string, 3 quotes to start and to end */
\"\"\"[^"""]*\"\"\" {
	std::string str = yytext;
	str = str.substr(3, str.size()-6);
	yylval->ident_value = new std::string(str);
	AdvanceCharacter(yyextra, strlen(yytext));
	return IDENT;
}

	/* python like string with single quotes */
'(\\.|[^(\\'|\n)])*' {
	UnescapedData data = UnEscapeString(yytext);
	for(VectorString::const_iterator it=data.errors.begin(); it!=data.errors.end(); ++it)
	{
		ReportError(yyextra, *it);
	}
	yylval->ident_value = new std::string(data.str);
	AdvanceCharacter(yyextra, strlen(yytext));
	return IDENT;
}

	/* regular number, both float and ints */
-?{number}+(\.{number}+)?[fF]? {
	yylval->ident_value = new std::string(yytext);
	AdvanceCharacter(yyextra, yyextra->ch+=strlen(yytext));
	return IDENT;
}

	/* hexadecimal */
0x[0-9a-fA-F]+ {
	yylval->ident_value = new std::string(yytext);
	AdvanceCharacter(yyextra, yyextra->ch+=strlen(yytext));
	return IDENT;
}

	/* binary */
0b[01]+ {
	yylval->ident_value = new std::string(yytext);
	AdvanceCharacter(yyextra, yyextra->ch+=strlen(yytext));
	return IDENT;
}

	/* heredoc parsing */
"<<" {
	int c;
	std::string name;
	int nameindex = -1;
	std::string data;
	std::string potential;
	int mystate = 0;
	bool done = false;
	bool eof = false;
	while(done==false) {
		if( done==false ) {
			c=yyinput(*yyextra->scanner);
		}
		if( c == 0 ) {
			done = true;
			eof = true;
		}
		if( c == EOF ) {
			done = true;
			eof = true;
		}
		if( done == false ) {
			switch(mystate) {
			case 0: // detect name
				if( c == ' ' || c == '\n' || c == '\t' ) {
					if( name.length() <= 0 ) {
						ReportError(yyextra, "EOF name is empty");
					}
					mystate = 1;
					unput(c);
				}
				else {
					name += c;
				}
				break;
			case 1: // name detected, ignore until newline
				if( c == '\n' ) {
					mystate = 2;
				}
				else {
					// ignore
				}
				break;
			case 2: // parse heredoc
				if( nameindex == -1 && c == '\n' ) {
					nameindex = 0;
				}
				else if( nameindex >= 0 && name[nameindex] == c) {
					potential += c;
					++nameindex;
					// static cast here to avoid compiler warning
					// this is probably safe as nameindex must be postive here
					assert(nameindex >= 0);
					if( static_cast<unsigned int>(nameindex) >= name.size() ) {
						// if the new index is beyond the string we have matched end string/name
						// can can continue to the last part and ignoring the end characters
						mystate = 3;
					}
				}
				else {
					if( nameindex >= 0 ) {
						data += "\n";
						data += potential;
						potential = "";
						nameindex = -1;
					}
					if( c == '\n' ) {
						nameindex = 0;
					}
					else {
						data += c;
					}
				}
				break;
			case 3:
				if( c == '\n' ) {
					mystate = 4;
				}
				else {
					// ignore
				}
				break;
			case 4:
				assert(0 && "shouldn't be here");
				break;
			}
		}

		if( mystate > 3) {
			done = true;
		}
	}
	if( mystate < 4 ) {
		if( eof ) {
			ReportError(yyextra, "Found EOF before heredoc end");
		}
	}
	if( eof ) {
		// adding 0 and EOF so that rest can fail without crashing
		// EOF only asserted
		unput(0);
		unput(EOF);
		yylval->ident_value = 0;
		return IDENT;
	}
	yylval->ident_value = new std::string(data);
	return IDENT;
}

[ \t] {
	AdvanceCharacter(yyextra, 1);
}

"\n" {
	++yyextra->line;
	ResetCharacter(yyextra);
}

. {
	AdvanceCharacter(yyextra, 1);
	std::stringstream ss;
	char c = yytext[0];
	int i = c;
	ss << "Unknown character [" << c << "(" << i << ")]";
	ReportError(yyextra, ss.str());
	return UNKNOWN;
}

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
			case '0': c = '\0'; break;
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

// http://stackoverflow.com/a/14678800
std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}