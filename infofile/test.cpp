#include <iostream>

#include "infofile/infofile.h"

void main() {
	infofile::Value val;
	infofile::Parse("data { key=value; }", &val);
}
