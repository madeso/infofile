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
		infofile::Parse("inline", "{key=value;}", &val, &errors);

		EXPECT_EQ(0, errors.size());
		ASSERT_EQ(1, val.children.size());

		EXPECT_EQ("key", val.children[0]->name());
		EXPECT_EQ("value", val.children[0]->value());
	}
}
