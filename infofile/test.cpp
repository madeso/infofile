#include <iostream>

#include "infofile/infofile.h"

void main() {
	std::vector<std::string> errors;
	infofile::Value val;
	infofile::Parse("inline", "@data { key=value; }", &val, &errors);
}
