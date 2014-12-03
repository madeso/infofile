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
		infofile::Node* val = infofile::Parse("inline", "{key=value;}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("key", val->name());
		EXPECT_EQ("value", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(testparsing_opass)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "{key value;}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("key", val->name());
		EXPECT_EQ("value", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(testparsing_osep)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "{key value}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("key", val->name());
		EXPECT_EQ("value", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(testparsing_twokeys)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "{key value k v}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(2, val->GetSibblingCount());

		EXPECT_EQ("key", val->name());
		EXPECT_EQ("value", val->value());

		EXPECT_TRUE(val->next != NULL);

		EXPECT_EQ("k", val->next->name());
		EXPECT_EQ("v", val->next->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(testparsing_array)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "[value v]", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(2, val->GetSibblingCount());

		EXPECT_EQ("", val->name());
		EXPECT_EQ("value", val->value());

		EXPECT_TRUE(val->next != NULL);
		EXPECT_EQ("", val->next->name());
		EXPECT_EQ("v", val->next->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(testparsing_array_sep_comma)
	{
		std::cout << "------------------------\n";
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "[value, v]", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(2, val->GetSibblingCount());

		EXPECT_EQ("", val->name());
		EXPECT_EQ("value", val->value());

		EXPECT_TRUE(val->next != NULL);
		EXPECT_EQ("", val->next->name());
		EXPECT_EQ("v", val->next->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(testparsing_array_sep_semicolon)
	{
		std::cout << "------------------------\n";
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "[value; v;]", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(2, val->GetSibblingCount());

		EXPECT_EQ("", val->name());
		EXPECT_EQ("value", val->value());

		EXPECT_TRUE(val->next != NULL);
		EXPECT_EQ("", val->next->name());
		EXPECT_EQ("v", val->next->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(testparsing_subkey)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "[{key value}]", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		ASSERT_TRUE(val != NULL);
		Node* n = val;

		EXPECT_EQ("", n->name());
		EXPECT_EQ("", n->value());
		EXPECT_TRUE(n->children != NULL);
		infofile::Node* v = n->children;
		ASSERT_EQ(1, v->GetSibblingCount());

		EXPECT_EQ("key", v->name());
		EXPECT_EQ("value", v->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(testparsing_subkey_multiple)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "[{a aa} {b bb}]", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(2, val->GetSibblingCount());

		Node* n;
		
		n = val;
		ASSERT_TRUE(val != NULL);
		EXPECT_EQ("", n->name());
		EXPECT_EQ("", n->value());
		ASSERT_EQ(1, n->GetChildCount());
		EXPECT_TRUE(n->children != NULL);
		EXPECT_EQ("a", n->children->name());
		EXPECT_EQ("aa", n->children->value());

		n = val->next;
		ASSERT_TRUE(val->next != NULL);
		EXPECT_EQ("", n->name());
		EXPECT_EQ("", n->value());
		ASSERT_EQ(1, n->GetChildCount());
		EXPECT_TRUE(n->children != NULL);
		EXPECT_EQ("b", n->children->name());
		EXPECT_EQ("bb", n->children->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(testparsing_subkey_multiple_comma)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "[{a aa}, {b bb}]", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(2, val->GetSibblingCount());

		Node* n;

		n = val;
		ASSERT_TRUE(val != NULL);
		EXPECT_EQ("", n->name());
		EXPECT_EQ("", n->value());
		ASSERT_EQ(1, n->GetChildCount());
		EXPECT_TRUE(n->children != NULL);
		EXPECT_EQ("a", n->children->name());
		EXPECT_EQ("aa", n->children->value());

		n = val->next;
		ASSERT_TRUE(val->next != NULL);
		EXPECT_EQ("", n->name());
		EXPECT_EQ("", n->value());
		ASSERT_EQ(1, n->GetChildCount());
		EXPECT_TRUE(n->children != NULL);
		EXPECT_EQ("b", n->children->name());
		EXPECT_EQ("bb", n->children->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(testparsing_subkey_multiple_semicolon)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "[{a aa}; {b bb};]", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(2, val->GetSibblingCount());

		Node* n;

		n = val;
		ASSERT_TRUE(val != NULL);
		EXPECT_EQ("", n->name());
		EXPECT_EQ("", n->value());
		ASSERT_EQ(1, n->GetChildCount());
		EXPECT_TRUE(n->children != NULL);
		EXPECT_EQ("a", n->children->name());
		EXPECT_EQ("aa", n->children->value());

		n = val->next;
		ASSERT_TRUE(val->next != NULL);
		EXPECT_EQ("", n->name());
		EXPECT_EQ("", n->value());
		ASSERT_EQ(1, n->GetChildCount());
		EXPECT_TRUE(n->children != NULL);
		EXPECT_EQ("b", n->children->name());
		EXPECT_EQ("bb", n->children->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_basic_string)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "{\"'key'\"=\"value\";}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("'key'", val->name());
		EXPECT_EQ("value", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_advanced_string)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "{\"key\\n\\t\"=\"value\\\"\";}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("key\n\t", val->name());
		EXPECT_EQ("value\"", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_basic_string_single)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "{'\"key\"'='value is nice';}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("\"key\"", val->name());
		EXPECT_EQ("value is nice", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_verbatim_string)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "{path @\"c:\\Docs\\Source\\a.txt\";}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("path", val->name());
		EXPECT_EQ("c:\\Docs\\Source\\a.txt", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_verbatim_string_tricky)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "{path @\"c:\\Docs\\Source\\\";}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("path", val->name());
		EXPECT_EQ("c:\\Docs\\Source\\", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_verbatim_double_quotes)
	{
		std::vector<std::string> errors;
		std::string src = "{line @\"\"\"Ahoy!\"\" cried the captain.\";}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("line", val->name());
		EXPECT_EQ("\"Ahoy!\" cried the captain.", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	//

	GTEST(test_verbatim_char)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "{path @'c:\\Docs\\Source\\a.txt';}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("path", val->name());
		EXPECT_EQ("c:\\Docs\\Source\\a.txt", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_verbatim_char_tricky)
	{
		std::vector<std::string> errors;
		infofile::Node* val = infofile::Parse("inline", "{path @'c:\\Docs\\Source\\\';}", &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("path", val->name());
		EXPECT_EQ("c:\\Docs\\Source\\", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_verbatim_char_double_quotes)
	{
		std::vector<std::string> errors;
		std::string src = "{line @'''Ahoy!'' cried the captain.';}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("line", val->name());
		EXPECT_EQ("'Ahoy!' cried the captain.", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_multiline_string_basic)
	{
		std::vector<std::string> errors;
		std::string src = "{line \"\"\"this is a long string\"\"\"}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("line", val->name());
		EXPECT_EQ("this is a long string", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_multiline_string_newlines)
	{
		std::vector<std::string> errors;
		std::string src = "{line \"\"\"this\nis\na\nlong\nstring\tright?\"\"\"}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("line", val->name());
		EXPECT_EQ("this\nis\na\nlong\nstring\tright?", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_newline_in_string_error)
	{
		std::vector<std::string> errors;
		std::string src = "{line \"hello\nworld\"}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		const unsigned int ZERO = 0;
		EXPECT_GT(errors.size(), ZERO);
		EXPECT_TRUE(val == NULL);
		
		if(val) delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}
	GTEST(test_newline_in_char_error)
	{
		std::vector<std::string> errors;
		std::string src = "{line 'hello\nworld'}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		const unsigned int ZERO = 0;
		EXPECT_GT(errors.size(), ZERO);
		EXPECT_TRUE(val == NULL);

		if (val) delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_newline_in_verbatim_string_error)
	{
		std::vector<std::string> errors;
		std::string src = "{line @\"hello\nworld\"}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		const unsigned int ZERO = 0;
		EXPECT_GT(errors.size(), ZERO);
		EXPECT_TRUE(val == NULL);

		if (val) delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}
	GTEST(test_newline_in_verbatim_char_error)
	{
		std::vector<std::string> errors;
		std::string src = "{line @'hello\nworld'}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		const unsigned int ZERO = 0;
		EXPECT_GT(errors.size(), ZERO);
		EXPECT_TRUE(val == NULL);

		if (val) delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_here_doc)
	{
		std::vector<std::string> errors;
		// when heredoc ends they ignore everything to the newline, therefore
		// we need the extra newline or the source will not parse and complain
		// about a EOF error
		std::string src = "{line <<EOF dog\nHello world EOF\ncat\nEOF dog=cat\n}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("line", val->name());
		EXPECT_EQ("Hello world EOF\ncat", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}
	GTEST(test_heredoc_error_eof)
	{
		std::vector<std::string> errors;

		std::string src = "{line <<EOF dog\nHello world EOF\ncat\nEOF dog=cat}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		const unsigned int ZERO = 0;
		EXPECT_GT(errors.size(), ZERO);
		EXPECT_TRUE(val == NULL);

		if (val) delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}
	GTEST(test_heredoc_error_noname)
	{
		std::vector<std::string> errors;

		std::string src = "{line <<EOF\ndog\nHello world EOF\ncat\nEOF dog=cat}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		const unsigned int ZERO = 0;
		EXPECT_GT(errors.size(), ZERO);
		EXPECT_TRUE(val == NULL);

		if (val) delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_singleline_comment)
	{
		std::vector<std::string> errors;
		std::string src = "{// this is a comment\nline dog}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("line", val->name());
		EXPECT_EQ("dog", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_multiline_comment_simple)
	{
		std::vector<std::string> errors;
		std::string src = "{line /*hello\nworld*/ dog}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("line", val->name());
		EXPECT_EQ("dog", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_multiline_comment_complex)
	{
		std::vector<std::string> errors;
		std::string src = "{line /***\nhello/* cat dog */\nworld ***/ dog}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("line", val->name());
		EXPECT_EQ("dog", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_combine)
	{
		std::vector<std::string> errors;
		std::string src = "{li + ne do \\ g}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("line", val->name());
		EXPECT_EQ("dog", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_root_struct)
	{
		std::vector<std::string> errors;
		std::string src = "line dog";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("line", val->name());
		EXPECT_EQ("dog", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_unicode_characters)
	{
		std::vector<std::string> errors;
		std::string src = "'ナ' 'ㄅ'";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("ナ", val->name());
		EXPECT_EQ("ㄅ", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_number_basic)
	{
		std::vector<std::string> errors;
		std::string src = "[ 12 ]";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("12", val->value());

		delete val;
#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_number_double)
	{
		std::vector<std::string> errors;
		std::string src = "[ 25.6 ]";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("25.6", val->value());

		delete val;
#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	/*
	Will have to investigate if we want to specify numbers this way, as it
	// seems that parsing might be harder if we do
	GTEST(test_double_start_with_dot)
	{
		std::vector<std::string> errors;
		std::string src = "[ .42 ]";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetChildCount());

		EXPECT_EQ(".42", val->value());

		delete val;
		#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
		#endif
	}
	*/

	GTEST(test_float)
	{
		std::vector<std::string> errors;
		std::string src = "[ 35f ]";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("35f", val->value());

		delete val;
#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_float_with_decimalpoint)
	{
		std::vector<std::string> errors;
		std::string src = "[ 12.3f ]";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("12.3f", val->value());

		delete val;
#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_advanced_ident)
	{
		std::vector<std::string> errors;
		std::string src = "jesus.opponent the.dude@gmail.com";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("jesus.opponent", val->name());
		EXPECT_EQ("the.dude@gmail.com", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_css_color)
	{
		std::vector<std::string> errors;
		std::string src = "#000 #12ffAA";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("#000", val->name());
		EXPECT_EQ("#12ffAA", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_underscore)
	{
		std::vector<std::string> errors;
		std::string src = "[hello_world]";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("hello_world", val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_zero_escape)
	{
		std::vector<std::string> errors;
		std::string src = "[\"hello\\0world\"]";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ(11, val->value().size());
		EXPECT_TRUE(0==memcmp("hello\0world", val->value().data(), 11));
		EXPECT_EQ(std::string("hello\0world", 11), val->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_empty_struct)
	{
		std::vector<std::string> errors;
		std::string src = "dog {}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);

		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("dog", val->name());
		EXPECT_EQ("", val->value());
		EXPECT_EQ(0, val->GetChildCount());
		ASSERT_TRUE(val->children == NULL);

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_empty_array)
	{
		std::vector<std::string> errors;
		std::string src = "dog []";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);
		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("dog", val->name());
		EXPECT_EQ("", val->value());
		EXPECT_EQ(0, val->GetChildCount());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_advanced_struct)
	{
		std::vector<std::string> errors;
		std::string src = "key value {a b}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);

		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("key", val->name());
		EXPECT_EQ("value", val->value());
		EXPECT_EQ(1, val->GetChildCount());
		ASSERT_TRUE(val->children != NULL);

		EXPECT_EQ("a", val->children->name());
		EXPECT_EQ("b", val->children->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	GTEST(test_advanced_struct_with_assign)
	{
		std::vector<std::string> errors;
		std::string src = "key : value := {a b}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);

		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("key", val->name());
		EXPECT_EQ("value", val->value());
		EXPECT_EQ(1, val->GetChildCount());
		ASSERT_TRUE(val->children != NULL);

		EXPECT_EQ("a", val->children->name());
		EXPECT_EQ("b", val->children->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}

	/*
	Possible bad typos could exist bc of this, probably shouldn't be allowed
	GTEST(test_advanced_struct_with_assign_and_empty_value)
	{
		std::vector<std::string> errors;
		std::string src = "key : := {a b}";
		infofile::Node* val = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		ASSERT_TRUE(val != NULL);

		ASSERT_EQ(1, val->GetSibblingCount());

		EXPECT_EQ("key", val->name());
		EXPECT_EQ("", val->value());
		EXPECT_EQ(1, val->GetChildCount());
		ASSERT_TRUE(val->children != NULL);

		EXPECT_EQ("a", val->children->name());
		EXPECT_EQ("b", val->children->value());

		delete val;

#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
#endif
	}
	*/

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
		infofile::Node* uni = infofile::Parse("inline", src, &errors);

		ASSERT_THAT(errors, testing::IsEmpty());
		EXPECT_TRUE(uni != NULL);
		ASSERT_EQ(1, uni->GetChildCount());

		#if INFOFILE_USE_BASIC_MEMCHECK
		EXPECT_EQ(0, Node::ActiveCount());
		#endif
	}
	*/
}
