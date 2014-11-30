#include <iostream>
#include "infofile.h"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Missing argument\n";
		return 1;
	}

	std::vector<std::string> errors;
	infofile::Value* val = infofile::ReadFile(argv[1], &errors);
	if (val != 0) delete val;
	for (std::vector<std::string>::iterator i = errors.begin(); i != errors.end(); ++i) {
		std::cerr << *i << "\n";
	}

	return 0;
}
