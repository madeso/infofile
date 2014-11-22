#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "infofile/infofile.h"

#define GTEST(x) TEST(INFOFILE, x)

namespace test
{
	using namespace infofile;

	GTEST(testparsing)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "{key=value;}", &errors);

		EXPECT_EQ(0, errors.size());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("key", val->children[0]->name());
		EXPECT_EQ("value", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(testparsing_opass)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "{key value;}", &errors);

		EXPECT_EQ(0, errors.size());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("key", val->children[0]->name());
		EXPECT_EQ("value", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(testparsing_osep)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "{key value}", &errors);

		EXPECT_EQ(0, errors.size());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("key", val->children[0]->name());
		EXPECT_EQ("value", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(testparsing_twokeys)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "{key value k v}", &errors);

		EXPECT_EQ(0, errors.size());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(2, val->children.size());

		EXPECT_EQ("key", val->children[0]->name());
		EXPECT_EQ("value", val->children[0]->value());

		EXPECT_EQ("k", val->children[1]->name());
		EXPECT_EQ("v", val->children[1]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(testparsing_array)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "[value v]", &errors);

		EXPECT_EQ(0, errors.size());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(2, val->children.size());

		EXPECT_EQ("", val->children[0]->name());
		EXPECT_EQ("value", val->children[0]->value());

		EXPECT_EQ("", val->children[1]->name());
		EXPECT_EQ("v", val->children[1]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(testparsing_subkey)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "[{key value}]", &errors);

		EXPECT_EQ(0, errors.size());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_TRUE(val->children[0] != NULL);
		Node* n = val->children[0];

		EXPECT_EQ("", n->name());
		EXPECT_EQ("", n->value());
		EXPECT_TRUE(n->children != NULL);
		Value* v = n->children;
		ASSERT_EQ(1, v->children.size());

		EXPECT_EQ("key", v->children[0]->name());
		EXPECT_EQ("value", v->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_basic_string)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "{\"'key'\"=\"value\";}", &errors);

		EXPECT_EQ(0, errors.size());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("'key'", val->children[0]->name());
		EXPECT_EQ("value", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_advanced_string)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "{\"key\\n\\t\"=\"value\\\"\";}", &errors);

		EXPECT_EQ(0, errors.size());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("key\n\t", val->children[0]->name());
		EXPECT_EQ("value\"", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_basic_string_single)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "{'\"key\"'='value is nice';}", &errors);

		EXPECT_EQ(0, errors.size());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("\"key\"", val->children[0]->name());
		EXPECT_EQ("value is nice", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_verbatim_string)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "{path @\"c:\\Docs\\Source\\a.txt\";}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("path", val->children[0]->name());
		EXPECT_EQ("c:\\Docs\\Source\\a.txt", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_verbatim_string_tricky)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "{path @\"c:\\Docs\\Source\\\";}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("path", val->children[0]->name());
		EXPECT_EQ("c:\\Docs\\Source\\", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_verbatim_double_quotes)
	{
		std::vector<std::string> errors;
		std::string src = "{line @\"\"\"Ahoy!\"\" cried the captain.\";}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("line", val->children[0]->name());
		EXPECT_EQ("\"Ahoy!\" cried the captain.", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	//

	GTEST(test_verbatim_char)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "{path @'c:\\Docs\\Source\\a.txt';}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("path", val->children[0]->name());
		EXPECT_EQ("c:\\Docs\\Source\\a.txt", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_verbatim_char_tricky)
	{
		std::vector<std::string> errors;
		infofile::Value* val = infofile::Parse("inline", "{path @'c:\\Docs\\Source\\\';}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("path", val->children[0]->name());
		EXPECT_EQ("c:\\Docs\\Source\\", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_verbatim_char_double_quotes)
	{
		std::vector<std::string> errors;
		std::string src = "{line @'''Ahoy!'' cried the captain.';}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("line", val->children[0]->name());
		EXPECT_EQ("'Ahoy!' cried the captain.", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}
}
