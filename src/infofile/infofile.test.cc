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
    REQUIRE(1 == val->children.size());

    CHECK("key" == val->name);
    CHECK("value" == val->value);
}

TEST_CASE("testparsing_opass")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{key value;}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("key" == val->children[0]->name);
    CHECK("value" == val->children[0]->value);
}

TEST_CASE("testparsing_osep")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{key value}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("key" == val->children[0]->name);
    CHECK("value" == val->children[0]->value);
}

TEST_CASE("testparsing_twokeys")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{key value k v}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->children.size());

    CHECK("key" == val->children[0]->name);
    CHECK("value" == val->children[0]->value);

    CHECK("k" == val->children[1]->name);
    CHECK("v" == val->children[1]->value);
}

TEST_CASE("testparsing_array")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[value v]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->children.size());

    CHECK("" == val->children[0]->name);
    CHECK("value" == val->children[0]->value);

    CHECK("" == val->children[1]->name);
    CHECK("v" == val->children[1]->value);
}

TEST_CASE("testparsing_array_sep_comma")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[value, v]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->children.size());

    CHECK("" == val->children[0]->name);
    CHECK("value" == val->children[0]->value);

    CHECK("" == val->children[1]->name);
    CHECK("v" == val->children[1]->value);
}

TEST_CASE("testparsing_array_sep_semicolon")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[value; v;]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->children.size());

    CHECK("" == val->children[0]->name);
    CHECK("value" == val->children[0]->value);

    CHECK("" == val->children[1]->name);
    CHECK("v" == val->children[1]->value);
}

TEST_CASE("testparsing_subkey")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[{key value}]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    REQUIRE(val != nullptr);
    std::shared_ptr<Node> n = val->children[0];

    CHECK("" == n->name);
    CHECK("" == n->value);
    REQUIRE(1 == n->children.size());

    CHECK("key" == n->children[0]->name);
    CHECK("value" == n->children[0]->value);
}

TEST_CASE("testparsing_subkey_multiple")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[{a aa} {b bb}]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->children.size());

    REQUIRE(val != nullptr);
    std::shared_ptr<Node> n = val->children[0];

    CHECK("" == n->name);
    CHECK("" == n->value);
    REQUIRE(2 == n->children.size());

    CHECK("a" == n->children[0]->name);
    CHECK("aa" == n->children[0]->value);

    CHECK("b" == n->children[1]->name);
    CHECK("bb" == n->children[1]->value);
}

TEST_CASE("testparsing_subkey_multiple_comma")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[{a aa}, {b bb}]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->children.size());

    REQUIRE(val != nullptr);
    std::shared_ptr<Node> n = val->children[0];

    CHECK("" == n->name);
    CHECK("" == n->value);
    REQUIRE(2 == n->children.size());

    CHECK("a" == n->children[0]->name);
    CHECK("aa" == n->children[0]->value);

    CHECK("b" == n->children[1]->name);
    CHECK("bb" == n->children[1]->value);
}

TEST_CASE("testparsing_subkey_multiple_semicolon")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "[{a aa}; {b bb};]", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(2 == val->children.size());

    REQUIRE(val != nullptr);
    std::shared_ptr<Node> n = val->children[0];

    CHECK("" == n->name);
    CHECK("" == n->value);
    REQUIRE(2 == n->children.size());

    CHECK("a" == n->children[0]->name);
    CHECK("aa" == n->children[0]->value);

    CHECK("b" == n->children[1]->name);
    CHECK("bb" == n->children[1]->value);
}

TEST_CASE("test_basic_string")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{\"'key'\"=\"value\";}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("'key'" == val->name);
    CHECK("value" == val->value);
}

TEST_CASE("test_advanced_string")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{\"key\\n\\t\"=\"value\\\"\";}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("key\n\t" == val->name);
    CHECK("value\"" == val->value);
}

TEST_CASE("test_basic_string_single")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{'\"key\"'='value is nice';}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("\"key\"" == val->name);
    CHECK("value is nice" == val->value);
}

TEST_CASE("test_verbatim_string")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{path @\"c:\\Docs\\Source\\a.txt\";}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("path" == val->name);
    CHECK("c:\\Docs\\Source\\a.txt" == val->value);
}

TEST_CASE("test_verbatim_string_tricky")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{path @\"c:\\Docs\\Source\\\";}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("path" == val->name);
    CHECK("c:\\Docs\\Source\\" == val->value);
}

TEST_CASE("test_verbatim_double_quotes")
{
    std::vector<std::string> errors;
    std::string src = "{line @\"\"\"Ahoy!\"\" cried the captain.\";}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("line" == val->name);
    CHECK("\"Ahoy!\" cried the captain." == val->value);
}

//

TEST_CASE("test_verbatim_char")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{path @'c:\\Docs\\Source\\a.txt';}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("path" == val->name);
    CHECK("c:\\Docs\\Source\\a.txt" == val->value);
}

TEST_CASE("test_verbatim_char_tricky")
{
    std::vector<std::string> errors;
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", "{path @'c:\\Docs\\Source\\\';}", &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("path" == val->name);
    CHECK("c:\\Docs\\Source\\" == val->value);
}

TEST_CASE("test_verbatim_char_double_quotes")
{
    std::vector<std::string> errors;
    std::string src = "{line @'''Ahoy!'' cried the captain.';}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("line" == val->name);
    CHECK("'Ahoy!' cried the captain." == val->value);
}

TEST_CASE("test_multiline_string_basic")
{
    std::vector<std::string> errors;
    std::string src = "{line \"\"\"this is a long string\"\"\"}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("line" == val->name);
    CHECK("this is a long string" == val->value);
}

TEST_CASE("test_multiline_string_newlines")
{
    std::vector<std::string> errors;
    std::string src = "{line \"\"\"this\nis\na\nlong\nstring\tright?\"\"\"}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("line" == val->name);
    CHECK("this\nis\na\nlong\nstring\tright?" == val->value);
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
    REQUIRE(1 == val->children.size());

    CHECK("line" == val->name);
    CHECK("Hello world EOF\ncat" == val->value);
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
    REQUIRE(1 == val->children.size());

    CHECK("line" == val->children[0]->name);
    CHECK("dog" == val->children[0]->value);
}

TEST_CASE("test_multiline_comment_simple")
{
    std::vector<std::string> errors;
    std::string src = "{line /*hello\nworld*/ dog}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("line" == val->children[0]->name);
    CHECK("dog" == val->children[0]->value);
}

TEST_CASE("test_multiline_comment_complex")
{
    std::vector<std::string> errors;
    std::string src = "{line /***\nhello/* cat dog */\nworld ***/ dog}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("line" == val->children[0]->name);
    CHECK("dog" == val->children[0]->value);
}

TEST_CASE("test_combine")
{
    std::vector<std::string> errors;
    std::string src = "{li + ne do \\ g}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("line" == val->children[0]->name);
    CHECK("dog" == val->children[0]->value);
}

TEST_CASE("test_root_struct")
{
    std::vector<std::string> errors;
    std::string src = "line dog";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(0 == val->children.size());

    CHECK("line" == val->name);
    CHECK("dog" == val->value);
}

TEST_CASE("test_unicode_characters")
{
    std::vector<std::string> errors;
    std::string src = "'ナ' 'ㄅ'";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(0 == val->children.size());

    CHECK("ナ" == val->name);
    CHECK("ㄅ" == val->value);
}

TEST_CASE("test_number_basic")
{
    std::vector<std::string> errors;
    std::string src = "[ 12 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("12" == val->children[0]->value);
}

TEST_CASE("test_number_double")
{
    std::vector<std::string> errors;
    std::string src = "[ 25.6 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("25.6" == val->children[0]->value);
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
    REQUIRE(1 == val->children.size());

    CHECK(".42" == val->children[0]->value);

}
*/

TEST_CASE("test_float")
{
    std::vector<std::string> errors;
    std::string src = "[ 35f ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("35f" == val->children[0]->value);
}

TEST_CASE("test_float_with_decimalpoint")
{
    std::vector<std::string> errors;
    std::string src = "[ 12.3f ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("12.3f" == val->children[0]->value);
}

//////////////////////////////////////////////////////////////////////////

TEST_CASE("test_negative_number_basic")
{
    std::vector<std::string> errors;
    std::string src = "[ -12 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("-12" == val->children[0]->value);
}

TEST_CASE("test_negative_number_double")
{
    std::vector<std::string> errors;
    std::string src = "[ -25.6 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("-25.6" == val->children[0]->value);
}

TEST_CASE("test_negative_float")
{
    std::vector<std::string> errors;
    std::string src = "[ -35f ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("-35f" == val->children[0]->value);
}

TEST_CASE("test_negative_float_with_decimalpoint")
{
    std::vector<std::string> errors;
    std::string src = "[ -12.3f ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("-12.3f" == val->children[0]->value);
}

TEST_CASE("test_advanced_ident")
{
    std::vector<std::string> errors;
    std::string src = "jesus.opponent the.dude@gmail.com";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(0 == val->children.size());

    CHECK("jesus.opponent" == val->name);
    CHECK("the.dude@gmail.com" == val->value);
}

TEST_CASE("test_css_color")
{
    std::vector<std::string> errors;
    std::string src = "#000 #12ffAA";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(0 == val->children.size());

    CHECK("#000" == val->name);
    CHECK("#12ffAA" == val->value);
}

TEST_CASE("test_underscore")
{
    std::vector<std::string> errors;
    std::string src = "[hello_world]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("hello_world" == val->children[0]->value);
}

TEST_CASE("test_zero_escape")
{
    std::vector<std::string> errors;
    std::string src = "[\"hello\\0world\"]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK(11 == val->value.size());
    REQUIRE(0 == std::memcmp("hello\0world", val->children[0]->value.data(), 11));
    CHECK(std::string("hello\0world", 11) == val->children[0]->value);
}

TEST_CASE("test_empty_struct")
{
    std::vector<std::string> errors;
    std::string src = "dog {}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    CHECK("dog" == val->name);
    CHECK("" == val->value);
    CHECK(0 == val->children.size());
}

TEST_CASE("test_empty_array")
{
    std::vector<std::string> errors;
    std::string src = "dog []";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    CHECK("dog" == val->name);
    CHECK("" == val->value);
    CHECK(0 == val->children.size());
}

TEST_CASE("test_advanced_struct")
{
    std::vector<std::string> errors;
    std::string src = "key value {a b}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    CHECK("key" == val->name);
    CHECK("value" == val->value);
    CHECK(1 == val->children.size());

    CHECK("a" == val->children[0]->name);
    CHECK("b" == val->children[0]->value);
}

TEST_CASE("test_advanced_struct_with_assign")
{
    std::vector<std::string> errors;
    std::string src = "key : value := {a b}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    REQUIRE(1 == val->children.size());

    CHECK("key" == val->name);
    CHECK("value" == val->value);
    CHECK(1 == val->children.size());

    CHECK("a" == val->children[0]->name);
    CHECK("b" == val->children[0]->value);
}

TEST_CASE("test_advanced_struct_with_assign_no_value")
{
    std::vector<std::string> errors;
    std::string src = "key := {a b}";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    REQUIRE(1 == val->children.size());

    CHECK("key" == val->name);
    CHECK("" == val->value);
    CHECK(1 == val->children.size());

    CHECK("a" == val->children[0]->name);
    CHECK("b" == val->children[0]->value);
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

    REQUIRE(1 == val->children.size());

    CHECK("key" == val->name);
    CHECK("" == val->value);
    CHECK(1 == val->children.size());
    REQUIRE(val->children != nullptr);

    CHECK("a" == val->children->name);
    CHECK("b" == val->children->value);


}
*/

TEST_CASE("test_advanced_array")
{
    std::vector<std::string> errors;
    std::string src = "key value [a]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    CHECK("key" == val->name);
    CHECK("value" == val->value);
    CHECK(1 == val->children.size());

    CHECK("" == val->children[0]->name);
    CHECK("a" == val->children[0]->value);
}

TEST_CASE("test_advanced_array_with_assign")
{
    std::vector<std::string> errors;
    std::string src = "key : value := [a]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    REQUIRE(1 == val->children.size());

    CHECK("key" == val->name);
    CHECK("value" == val->value);
    CHECK(1 == val->children.size());

    CHECK("" == val->children[0]->name);
    CHECK("a" == val->children[0]->value);
}

TEST_CASE("test_advanced_array_with_assign_no_value")
{
    std::vector<std::string> errors;
    std::string src = "key := [a]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);

    REQUIRE(1 == val->children.size());

    CHECK("key" == val->name);
    CHECK("" == val->value);
    CHECK(1 == val->children.size());

    CHECK("" == val->children[0]->name);
    CHECK("a" == val->children[0]->value);
}

TEST_CASE("test_octal")
{
    std::vector<std::string> errors;
    std::string src = "[ 0042 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("0042" == val->children[0]->value);
}

TEST_CASE("test_hexadecimal")
{
    std::vector<std::string> errors;
    std::string src = "[ 0xaeF2 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("0xaeF2" == val->children[0]->value);
}

TEST_CASE("test_binary")
{
    std::vector<std::string> errors;
    std::string src = "[ 0b00010000 ]";
    std::shared_ptr<infofile::Node> val = infofile::Parse("inline", src, &errors);

    REQUIRE(IsEqual(errors, {}));
    REQUIRE(val != nullptr);
    REQUIRE(1 == val->children.size());

    CHECK("0b00010000" == val->children[0]->value);
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
    REQUIRE(1 == uni->children.size());

}
*/
