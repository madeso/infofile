#include <cstring>

#include "catch.hpp"
#include "infofile/infofile.h"

using namespace infofile;

bool IsEqual(const std::vector<std::string>& errors, const std::vector<std::string>& right)
{
    // todo(Gustav): use catchy here
    return false;
}

TEST_CASE("testparsing")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{key=value;}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("key" == val->name());
    CHECK("value" == val->value());
}

TEST_CASE("testparsing_opass")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{key value;}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("key" == val->name());
    CHECK("value" == val->value());
}

TEST_CASE("testparsing_osep")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{key value}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("key" == val->name());
    CHECK("value" == val->value());
}

TEST_CASE("testparsing_twokeys")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{key value k v}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->GetSibblingCount());

    CHECK("key" == val->name());
    CHECK("value" == val->value());

    REQUIRE(val->next != nullptr);

    CHECK("k" == val->next->name());
    CHECK("v" == val->next->value());
}

TEST_CASE("testparsing_array")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[value v]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->GetSibblingCount());

    CHECK("" == val->name());
    CHECK("value" == val->value());

    REQUIRE(val->next != nullptr);
    CHECK("" == val->next->name());
    CHECK("v" == val->next->value());
}

TEST_CASE("testparsing_array_sep_comma")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[value, v]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->GetSibblingCount());

    CHECK("" == val->name());
    CHECK("value" == val->value());

    REQUIRE(val->next != nullptr);
    CHECK("" == val->next->name());
    CHECK("v" == val->next->value());
}

TEST_CASE("testparsing_array_sep_semicolon")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[value; v;]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->GetSibblingCount());

    CHECK("" == val->name());
    CHECK("value" == val->value());

    REQUIRE(val->next != nullptr);
    CHECK("" == val->next->name());
    CHECK("v" == val->next->value());
}

TEST_CASE("testparsing_subkey")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[{key value}]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    REQUIRE(val != nullptr);
    std::shared_ptr<Node> n = val;

    CHECK("" == n->name());
    CHECK("" == n->value());
    REQUIRE(n->children != nullptr);
    std::shared_ptr<infofile::Node> v = n->children;
    REQUIRE(1 == v->GetSibblingCount());

    CHECK("key" == v->name());
    CHECK("value" == v->value());
}

TEST_CASE("testparsing_subkey_multiple")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[{a aa} {b bb}]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->GetSibblingCount());

    std::shared_ptr<Node> n;

    n = val;
    REQUIRE(val != nullptr);
    CHECK("" == n->name());
    CHECK("" == n->value());
    REQUIRE(1 == n->GetChildCount());
    REQUIRE(n->children != nullptr);
    CHECK("a" == n->children->name());
    CHECK("aa" == n->children->value());

    n = val->next;
    REQUIRE(val->next != nullptr);
    CHECK("" == n->name());
    CHECK("" == n->value());
    REQUIRE(1 == n->GetChildCount());
    REQUIRE(n->children != nullptr);
    CHECK("b" == n->children->name());
    CHECK("bb" == n->children->value());
}

TEST_CASE("testparsing_subkey_multiple_comma")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[{a aa}, {b bb}]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->GetSibblingCount());

    std::shared_ptr<Node> n;

    n = val;
    REQUIRE(val != nullptr);
    CHECK("" == n->name());
    CHECK("" == n->value());
    REQUIRE(1 == n->GetChildCount());
    REQUIRE(n->children != nullptr);
    CHECK("a" == n->children->name());
    CHECK("aa" == n->children->value());

    n = val->next;
    REQUIRE(val->next != nullptr);
    CHECK("" == n->name());
    CHECK("" == n->value());
    REQUIRE(1 == n->GetChildCount());
    REQUIRE(n->children != nullptr);
    CHECK("b" == n->children->name());
    CHECK("bb" == n->children->value());
}

TEST_CASE("testparsing_subkey_multiple_semicolon")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[{a aa}; {b bb};]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->GetSibblingCount());

    std::shared_ptr<Node> n;

    n = val;
    REQUIRE(val != nullptr);
    CHECK("" == n->name());
    CHECK("" == n->value());
    REQUIRE(1 == n->GetChildCount());
    REQUIRE(n->children != nullptr);
    CHECK("a" == n->children->name());
    CHECK("aa" == n->children->value());

    n = val->next;
    REQUIRE(val->next != nullptr);
    CHECK("" == n->name());
    CHECK("" == n->value());
    REQUIRE(1 == n->GetChildCount());
    REQUIRE(n->children != nullptr);
    CHECK("b" == n->children->name());
    CHECK("bb" == n->children->value());
}

TEST_CASE("test_basic_string")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{\"'key'\"=\"value\";}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("'key'" == val->name());
    CHECK("value" == val->value());
}

TEST_CASE("test_advanced_string")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{\"key\\n\\t\"=\"value\\\"\";}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("key\n\t" == val->name());
    CHECK("value\"" == val->value());
}

TEST_CASE("test_basic_string_single")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{'\"key\"'='value is nice';}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("\"key\"" == val->name());
    CHECK("value is nice" == val->value());
}

TEST_CASE("test_verbatim_string")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{path @\"c:\\Docs\\Source\\a.txt\";}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("path" == val->name());
    CHECK("c:\\Docs\\Source\\a.txt" == val->value());
}

TEST_CASE("test_verbatim_string_tricky")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{path @\"c:\\Docs\\Source\\\";}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("path" == val->name());
    CHECK("c:\\Docs\\Source\\" == val->value());
}

TEST_CASE("test_verbatim_double_quotes")
{
    std::vector<std::string> errors;
    std::string src = "{line @\"\"\"Ahoy!\"\" cried the captain.\";}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("line" == val->name());
    CHECK("\"Ahoy!\" cried the captain." == val->value());
}

//

TEST_CASE("test_verbatim_char")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{path @'c:\\Docs\\Source\\a.txt';}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("path" == val->name());
    CHECK("c:\\Docs\\Source\\a.txt" == val->value());
}

TEST_CASE("test_verbatim_char_tricky")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{path @'c:\\Docs\\Source\\\';}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("path" == val->name());
    CHECK("c:\\Docs\\Source\\" == val->value());
}

TEST_CASE("test_verbatim_char_double_quotes")
{
    std::vector<std::string> errors;
    std::string src = "{line @'''Ahoy!'' cried the captain.';}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("line" == val->name());
    CHECK("'Ahoy!' cried the captain." == val->value());
}

TEST_CASE("test_multiline_string_basic")
{
    std::vector<std::string> errors;
    std::string src = "{line \"\"\"this is a long string\"\"\"}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("line" == val->name());
    CHECK("this is a long string" == val->value());
}

TEST_CASE("test_multiline_string_newlines")
{
    std::vector<std::string> errors;
    std::string src = "{line \"\"\"this\nis\na\nlong\nstring\tright?\"\"\"}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("line" == val->name());
    CHECK("this\nis\na\nlong\nstring\tright?" == val->value());
}

TEST_CASE("test_newline_in_string_error")
{
    std::vector<std::string> errors;
    std::string src = "{line \"hello\nworld\"}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    const unsigned int ZERO = 0;
    REQUIRE(errors.size() > ZERO);
    REQUIRE(val == nullptr);
}
TEST_CASE("test_newline_in_char_error")
{
    std::vector<std::string> errors;
    std::string src = "{line 'hello\nworld'}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    const unsigned int ZERO = 0;
    REQUIRE(errors.size() > ZERO);
    REQUIRE(val == nullptr);
}

TEST_CASE("test_newline_in_verbatim_string_error")
{
    std::vector<std::string> errors;
    std::string src = "{line @\"hello\nworld\"}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    const unsigned int ZERO = 0;
    REQUIRE(errors.size() > ZERO);
    REQUIRE(val == nullptr);
}
TEST_CASE("test_newline_in_verbatim_char_error")
{
    std::vector<std::string> errors;
    std::string src = "{line @'hello\nworld'}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    const unsigned int ZERO = 0;
    REQUIRE(errors.size() > ZERO);
    REQUIRE(val == nullptr);
}

TEST_CASE("test_here_doc")
{
    std::vector<std::string> errors;
    // when heredoc ends they ignore everything to the newline, therefore
    // we need the extra newline or the source will not parse and complain
    // about a EOF error
    std::string src = "{line <<EOF dog\nHello world EOF\ncat\nEOF dog=cat\n}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("line" == val->name());
    CHECK("Hello world EOF\ncat" == val->value());
}
TEST_CASE("test_heredoc_error_eof")
{
    std::vector<std::string> errors;

    std::string src = "{line <<EOF dog\nHello world EOF\ncat\nEOF dog=cat}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    const unsigned int ZERO = 0;
    REQUIRE(errors.size() > ZERO);
    REQUIRE(val == nullptr);
}
TEST_CASE("test_heredoc_error_noname")
{
    std::vector<std::string> errors;

    std::string src = "{line <<EOF\ndog\nHello world EOF\ncat\nEOF dog=cat}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    const unsigned int ZERO = 0;
    REQUIRE(errors.size() > ZERO);
    REQUIRE(val == nullptr);
}

TEST_CASE("test_singleline_comment")
{
    std::vector<std::string> errors;
    std::string src = "{// this is a comment\nline dog}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("line" == val->name());
    CHECK("dog" == val->value());
}

TEST_CASE("test_multiline_comment_simple")
{
    std::vector<std::string> errors;
    std::string src = "{line /*hello\nworld*/ dog}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("line" == val->name());
    CHECK("dog" == val->value());
}

TEST_CASE("test_multiline_comment_complex")
{
    std::vector<std::string> errors;
    std::string src = "{line /***\nhello/* cat dog */\nworld ***/ dog}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("line" == val->name());
    CHECK("dog" == val->value());
}

TEST_CASE("test_combine")
{
    std::vector<std::string> errors;
    std::string src = "{li + ne do \\ g}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("line" == val->name());
    CHECK("dog" == val->value());
}

TEST_CASE("test_root_struct")
{
    std::vector<std::string> errors;
    std::string src = "line dog";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("line" == val->name());
    CHECK("dog" == val->value());
}

TEST_CASE("test_unicode_characters")
{
    std::vector<std::string> errors;
    std::string src = "'ナ' 'ㄅ'";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("ナ" == val->name());
    CHECK("ㄅ" == val->value());
}

TEST_CASE("test_number_basic")
{
    std::vector<std::string> errors;
    std::string src = "[ 12 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("12" == val->value());
}

TEST_CASE("test_number_double")
{
    std::vector<std::string> errors;
    std::string src = "[ 25.6 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("25.6" == val->value());
}

/*
Will have to investigate if we want to specify numbers this way, as it
// seems that parsing might be harder if we do
TEST_CASE("test_double_start_with_dot")
{
    std::vector<std::string> errors;
    std::string src = "[ .42 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetChildCount());

    CHECK(".42" == val->value());

}
*/

TEST_CASE("test_float")
{
    std::vector<std::string> errors;
    std::string src = "[ 35f ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("35f" == val->value());
}

TEST_CASE("test_float_with_decimalpoint")
{
    std::vector<std::string> errors;
    std::string src = "[ 12.3f ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("12.3f" == val->value());
}

//////////////////////////////////////////////////////////////////////////

TEST_CASE("test_negative_number_basic")
{
    std::vector<std::string> errors;
    std::string src = "[ -12 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("-12" == val->value());
}

TEST_CASE("test_negative_number_double")
{
    std::vector<std::string> errors;
    std::string src = "[ -25.6 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("-25.6" == val->value());
}

TEST_CASE("test_negative_float")
{
    std::vector<std::string> errors;
    std::string src = "[ -35f ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("-35f" == val->value());
}

TEST_CASE("test_negative_float_with_decimalpoint")
{
    std::vector<std::string> errors;
    std::string src = "[ -12.3f ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("-12.3f" == val->value());
}

TEST_CASE("test_advanced_ident")
{
    std::vector<std::string> errors;
    std::string src = "jesus.opponent the.dude@gmail.com";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("jesus.opponent" == val->name());
    CHECK("the.dude@gmail.com" == val->value());
}

TEST_CASE("test_css_color")
{
    std::vector<std::string> errors;
    std::string src = "#000 #12ffAA";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("#000" == val->name());
    CHECK("#12ffAA" == val->value());
}

TEST_CASE("test_underscore")
{
    std::vector<std::string> errors;
    std::string src = "[hello_world]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("hello_world" == val->value());
}

TEST_CASE("test_zero_escape")
{
    std::vector<std::string> errors;
    std::string src = "[\"hello\\0world\"]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK(11 == val->value().size());
    REQUIRE(0 == std::memcmp("hello\0world", val->value().data(), 11));
    CHECK(std::string("hello\0world", 11) == val->value());
}

TEST_CASE("test_empty_struct")
{
    std::vector<std::string> errors;
    std::string src = "dog {}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    REQUIRE(1 == val->GetSibblingCount());

    CHECK("dog" == val->name());
    CHECK("" == val->value());
    CHECK(0 == val->GetChildCount());
    REQUIRE(val->children == nullptr);
}

TEST_CASE("test_empty_array")
{
    std::vector<std::string> errors;
    std::string src = "dog []";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("dog" == val->name());
    CHECK("" == val->value());
    CHECK(0 == val->GetChildCount());
}

TEST_CASE("test_advanced_struct")
{
    std::vector<std::string> errors;
    std::string src = "key value {a b}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    REQUIRE(1 == val->GetSibblingCount());

    CHECK("key" == val->name());
    CHECK("value" == val->value());
    CHECK(1 == val->GetChildCount());
    REQUIRE(val->children != nullptr);

    CHECK("a" == val->children->name());
    CHECK("b" == val->children->value());
}

TEST_CASE("test_advanced_struct_with_assign")
{
    std::vector<std::string> errors;
    std::string src = "key : value := {a b}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    REQUIRE(1 == val->GetSibblingCount());

    CHECK("key" == val->name());
    CHECK("value" == val->value());
    CHECK(1 == val->GetChildCount());
    REQUIRE(val->children != nullptr);

    CHECK("a" == val->children->name());
    CHECK("b" == val->children->value());
}

TEST_CASE("test_advanced_struct_with_assign_no_value")
{
    std::vector<std::string> errors;
    std::string src = "key := {a b}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    REQUIRE(1 == val->GetSibblingCount());

    CHECK("key" == val->name());
    CHECK("" == val->value());
    CHECK(1 == val->GetChildCount());
    REQUIRE(val->children != nullptr);

    CHECK("a" == val->children->name());
    CHECK("b" == val->children->value());
}

/*
Possible bad typos could exist bc of this, probably shouldn't be allowed
TEST_CASE("test_advanced_struct_with_assign_and_empty_value")
{
    std::vector<std::string> errors;
    std::string src = "key : := {a b}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    REQUIRE(1 == val->GetSibblingCount());

    CHECK("key" == val->name());
    CHECK("" == val->value());
    CHECK(1 == val->GetChildCount());
    REQUIRE(val->children != nullptr);

    CHECK("a" == val->children->name());
    CHECK("b" == val->children->value());


}
*/

TEST_CASE("test_advanced_array")
{
    std::vector<std::string> errors;
    std::string src = "key value [a]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    REQUIRE(1 == val->GetSibblingCount());

    CHECK("key" == val->name());
    CHECK("value" == val->value());
    CHECK(1 == val->GetChildCount());
    REQUIRE(val->children != nullptr);

    CHECK("" == val->children->name());
    CHECK("a" == val->children->value());
}

TEST_CASE("test_advanced_array_with_assign")
{
    std::vector<std::string> errors;
    std::string src = "key : value := [a]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    REQUIRE(1 == val->GetSibblingCount());

    CHECK("key" == val->name());
    CHECK("value" == val->value());
    CHECK(1 == val->GetChildCount());
    REQUIRE(val->children != nullptr);

    CHECK("" == val->children->name());
    CHECK("a" == val->children->value());
}

TEST_CASE("test_advanced_array_with_assign_no_value")
{
    std::vector<std::string> errors;
    std::string src = "key := [a]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    REQUIRE(1 == val->GetSibblingCount());

    CHECK("key" == val->name());
    CHECK("" == val->value());
    CHECK(1 == val->GetChildCount());
    REQUIRE(val->children != nullptr);

    CHECK("" == val->children->name());
    CHECK("a" == val->children->value());
}

TEST_CASE("test_octal")
{
    std::vector<std::string> errors;
    std::string src = "[ 0042 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("0042" == val->value());
}

TEST_CASE("test_hexadecimal")
{
    std::vector<std::string> errors;
    std::string src = "[ 0xaeF2 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("0xaeF2" == val->value());
}

TEST_CASE("test_binary")
{
    std::vector<std::string> errors;
    std::string src = "[ 0b00010000 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->GetSibblingCount());

    CHECK("0b00010000" == val->value());
}

/*
// todo: implement unicode escape characters
// taken from here https://github.com/dropbox/json11/blob/master/test.cpp
TEST_CASE("test_unicode_escape")
{
    const std::string src =
        R"([ "blah\ud83d\udca9blah\ud83dblah\udca9blah\u0000blah\u1234" ])";
    const char utf8[] = "blah" "\xf0\x9f\x92\xa9" "blah" "\xed\xa0\xbd" "blah"
        "\xed\xb2\xa9" "blah" "\0" "blah" "\xe1\x88\xb4";
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> uni = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(uni != nullptr);
    REQUIRE(1 == uni->GetChildCount());

}
*/
