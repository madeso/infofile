I got tired of different parsing libraries, weird specifications and strange file formats, so I made my own ([obligatory xkcd](http://xkcd.com/927/)) and licensed it under [zlib](https://tldrlegal.com/license/zlib-libpng-license-%28zlib%29).
This was initially inspired by the [boost info file](http://www.boost.org/doc/libs/1_56_0/doc/html/boost_propertytree/parsers.html#boost_propertytree.parsers.info_parser), hence the name, however have been major changes and boost is not needed.

The general structure is inspired by json/xml but with a syntax inspired by C, C++ etc.

Introduction:
==============

There are 2 core elements of the info file. The string and the node. Everything else is syntactic sugar(and there is much sugar).

One way of defining strings is the standard C/C++ like *"hello world"*, another is the enchaned identifiers like *nice_cat* or *some.email.com*.

The node consists of a name, a value, and a list of child nodes. All are optional and can be empty. There are 3 ways to specify the child nodes. Structure, value-array and structure array.

```cpp
name = "Gustav"; // a node without a structure
```

Structures allow the most flexibility and let's you specify how it's children are specified, some may have a name and no value while some may have children.

```cpp
developer { name: "Gustav"; } // a "struct node"
```

Array are syntactic sugar for structures, and only allows you to specify one specific structure with less typing. Value-arrays only let's specify the values of children while structure-array only lets you specify structures.

```cpp
ints = [42, 42, 42]; // a "value array"
```

A info file has a single root node, and neither name nor the value can be specified so it's per definition always empty.

```cpp
[ 1 2 3 ] // a array as a root node
{ name: Gustav; "on github": yes } // a strucutre as a root node
```

Typing brackets as the root node is tiring, so for the root node they are optional

```cpp
name: Gustav;
"on github": yes;
```

Example files:
==============

This is a basic info file:

```cpp
key1 value1
key2
{
    key3 value3
    {
        key4 "value4 with spaces"
    }
    key5 value5
}
```

Here is a more complicated file demonstrating all the info features:

```cpp
// A comment
key1 value1   /* Another comment */
key2 "value with special characters in it {};#\n\t\"\0"
{
    subkey "value split "\
            "over three"\
            "lines"
    {
        a_key_without_value ""
        "a key with special characters in it {};#\n\t\"\0" ""
        "" value    // Empty key with a value
        "" ""       // Empty key with empty value!
    }
}
mykey {} // when using {} the value can be ignored
names [ cat dog duck ] // a array.
```

Syntactic sugar:
=================

Assignment operator exist but are optional, the following nodes are equivalent:

```cpp
key value
key = value
key := value
key : value
```

Assignments are also allowed between the name, value and child nodes:

```cpp
key = value : {key value}
key : {key value}
```

There are also optional line terminators, these are also equivalent (spaces added for clarity):

```cpp
key value
key value ,
key value ;
```

Strings:
=========

Strings can be specified in various ways:

```
test ident
test "c string"
test 'python string'
test @"c:\temp\"
test @'c:\temp\'
test """multiline"""
test '''multiline'''
test <<EOF ignored data
    heredoc example
    also called here script
    or hereis according [to wikipedia](http://en.wikipedia.org/wiki/Here_document)
EOF also ignored
test #aaa
test #0F0F0F
test 42
test 12.0f
```

Strings can be combined with the c like operator \ or the java/C# like +

```cpp
test = "hello" + " world";
test = "hello" \ // no newline is necessary here but entered here on convenience.
        " world"
```

Remember that it's all string, all the time, so the following nodes are equal:

```cpp
same "123"
same """123"""
same 123
same 1 + 2 + 3
```

But not the same as:

```cpp
same 123.0
same 123.0f
```

Converting the string to a number will result in the same however, depending on the string parsing.


Using the library:
==================

The library is distributed in source form. You add it to your project by including the cmake file and use the generated library.

```cmake
add_subdirectory("path_to_infofile)
target_link_libraries(my_target PUBLIC infofile)
```

```cpp
#include "infofile/infofile.h"

std::vector<std::string> errors;
std::shared_ptr<infofile::Node> val = infofile::ReadFile("my_file.info", &errors);
if(val->children.empty()) { return false; }
std::cout << val->children[0]->name << " is " << val->children[0]->value << "\n";
```


Todo:
=======

This is a not yet completed library. Please see the [issues](https://github.com/madeso/infofile/issues?q=is%3Aopen+is%3Aissue).
