#include <gtest/gtest.h>

#include "infofile/infofile.h"

#define GTEST(x) TEST(INFOFILE, x)

namespace test
{
	using namespace infofile;

	GTEST(testparsing)
	{
		std::vector<std::string> errors;
		infofile::Value val;
		infofile::Parse("inline", "@data { key=value; }", &val, &errors);

		EXPECT_EQ(0, errors.size());
	}
}
