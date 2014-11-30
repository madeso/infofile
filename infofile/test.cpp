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

		ASSERT_THAT(errors, testing::IsEmpty());
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

		ASSERT_THAT(errors, testing::IsEmpty());
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

		ASSERT_THAT(errors, testing::IsEmpty());
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

		ASSERT_THAT(errors, testing::IsEmpty());
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

		ASSERT_THAT(errors, testing::IsEmpty());
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

		ASSERT_THAT(errors, testing::IsEmpty());
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

		ASSERT_THAT(errors, testing::IsEmpty());
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

		ASSERT_THAT(errors, testing::IsEmpty());
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

		ASSERT_THAT(errors, testing::IsEmpty());
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

	GTEST(test_multiline_string_basic)
	{
		std::vector<std::string> errors;
		std::string src = "{line \"\"\"this is a long string\"\"\"}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("line", val->children[0]->name());
		EXPECT_EQ("this is a long string", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_multiline_string_newlines)
	{
		std::vector<std::string> errors;
		std::string src = "{line \"\"\"this\nis\na\nlong\nstring\tright?\"\"\"}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("line", val->children[0]->name());
		EXPECT_EQ("this\nis\na\nlong\nstring\tright?", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_newline_in_string_error)
	{
		std::vector<std::string> errors;
		std::string src = "{line \"hello\nworld\"}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		const unsigned int ZERO = 0;
		EXPECT_GT(errors.size(), ZERO);
		EXPECT_TRUE(val == NULL);
		
		if(val) delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}
	GTEST(test_newline_in_char_error)
	{
		std::vector<std::string> errors;
		std::string src = "{line 'hello\nworld'}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		const unsigned int ZERO = 0;
		EXPECT_GT(errors.size(), ZERO);
		EXPECT_TRUE(val == NULL);

		if (val) delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_newline_in_verbatim_string_error)
	{
		std::vector<std::string> errors;
		std::string src = "{line @\"hello\nworld\"}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		const unsigned int ZERO = 0;
		EXPECT_GT(errors.size(), ZERO);
		EXPECT_TRUE(val == NULL);

		if (val) delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}
	GTEST(test_newline_in_verbatim_char_error)
	{
		std::vector<std::string> errors;
		std::string src = "{line @'hello\nworld'}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		const unsigned int ZERO = 0;
		EXPECT_GT(errors.size(), ZERO);
		EXPECT_TRUE(val == NULL);

		if (val) delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_here_doc)
	{
		std::vector<std::string> errors;
		// when heredoc ends they ignore everything to the newline, therefore
		// we need the extra newline or the source will not parse and complain
		// about a EOF error
		std::string src = "{line <<EOF dog\nHello world EOF\ncat\nEOF dog=cat\n}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("line", val->children[0]->name());
		EXPECT_EQ("Hello world EOF\ncat", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}
	GTEST(test_heredoc_error_eof)
	{
		std::vector<std::string> errors;

		std::string src = "{line <<EOF dog\nHello world EOF\ncat\nEOF dog=cat}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		const unsigned int ZERO = 0;
		EXPECT_GT(errors.size(), ZERO);
		EXPECT_TRUE(val == NULL);

		if (val) delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}
	GTEST(test_heredoc_error_noname)
	{
		std::vector<std::string> errors;

		std::string src = "{line <<EOF\ndog\nHello world EOF\ncat\nEOF dog=cat}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		const unsigned int ZERO = 0;
		EXPECT_GT(errors.size(), ZERO);
		EXPECT_TRUE(val == NULL);

		if (val) delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_singleline_comment)
	{
		std::vector<std::string> errors;
		std::string src = "{// this is a comment\nline dog}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("line", val->children[0]->name());
		EXPECT_EQ("dog", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_multiline_comment_simple)
	{
		std::vector<std::string> errors;
		std::string src = "{line /*hello\nworld*/ dog}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("line", val->children[0]->name());
		EXPECT_EQ("dog", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_multiline_comment_complex)
	{
		std::vector<std::string> errors;
		std::string src = "{line /***\nhello/* cat dog */\nworld ***/ dog}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("line", val->children[0]->name());
		EXPECT_EQ("dog", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_combine)
	{
		std::vector<std::string> errors;
		std::string src = "{li + ne do \\ g}";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("line", val->children[0]->name());
		EXPECT_EQ("dog", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_root_struct)
	{
		std::vector<std::string> errors;
		std::string src = "line dog";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("line", val->children[0]->name());
		EXPECT_EQ("dog", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_unicode_characters)
	{
		std::vector<std::string> errors;
		std::string src = "'ナ' 'ㄅ'";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("ナ", val->children[0]->name());
		EXPECT_EQ("ㄅ", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_number_basic)
	{
		std::vector<std::string> errors;
		std::string src = "[ 12 ]";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("12", val->children[0]->value());

		delete val;
		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_number_double)
	{
		std::vector<std::string> errors;
		std::string src = "[ 25.6 ]";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("25.6", val->children[0]->value());

		delete val;
		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	/*
	Will have to investigate if we want to specify numbers this way, as it
	// seems that parsing might be harder if we do
	GTEST(test_double_start_with_dot)
	{
		std::vector<std::string> errors;
		std::string src = "[ .42 ]";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ(".42", val->children[0]->value());

		delete val;
		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}
	*/

	GTEST(test_float)
	{
		std::vector<std::string> errors;
		std::string src = "[ 35f ]";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("35f", val->children[0]->value());

		delete val;
		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_float_with_decimalpoint)
	{
		std::vector<std::string> errors;
		std::string src = "[ 12.3f ]";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("12.3f", val->children[0]->value());

		delete val;
		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_advanced_ident)
	{
		std::vector<std::string> errors;
		std::string src = "jesus.opponent the.dude@gmail.com";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("jesus.opponent", val->children[0]->name());
		EXPECT_EQ("the.dude@gmail.com", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_css_color)
	{
		std::vector<std::string> errors;
		std::string src = "#000 #12ffAA";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("#000", val->children[0]->name());
		EXPECT_EQ("#12ffAA", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_underscore)
	{
		std::vector<std::string> errors;
		std::string src = "[hello_world]";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("hello_world", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	GTEST(test_zero_escape)
	{
		std::vector<std::string> errors;
		std::string src = "[\"hello\\0world\"]";
		infofile::Value* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(val != NULL);
		ASSERT_EQ(1, val->children.size());

		EXPECT_EQ("hello\0world", val->children[0]->value());

		delete val;

		EXPECT_EQ(0, Node::ActiveCount());
		EXPECT_EQ(0, Value::ActiveCount());
	}

	/*
	// todo: implement unicode escape characters
	// taken from here https://github.com/dropbox/json11/blob/master/test.cpp
	GTEST(test_unicode_escape)
	{
		const std::string src =
			R"([ "blah\ud83d\udca9blah\ud83dblah\udca9blah\u0000blah\u1234" ])";
		const char utf8[] = "blah" "\xf0\x9f\x92\xa9" "blah" "\xed\xa0\xbd" "blah"
			"\xed\xb2\xa9" "blah" "\0" "blah" "\xe1\x88\xb4";
		std::vector<std::string> errors;
		infofile::Value* uni = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(uni != NULL);
		ASSERT_EQ(1, uni->children.size());

		EXPECT_TRUE(uni->children[0]->value().size() == (sizeof utf8) - 1);
		EXPECT_TRUE(memcmp(uni->children[0]->value().data(), utf8, sizeof utf8) == 0);
	}
	*/
}
