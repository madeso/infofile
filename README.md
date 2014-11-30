I got tired of different parsing libraries, weird specifications and strange file formats, so I made my own ([obligatory xkcd](http://xkcd.com/927/)) and licensed it under [zlib](https://tldrlegal.com/license/zlib-libpng-license-%28zlib%29). This is based on the [boost info file](http://www.boost.org/doc/libs/1_56_0/doc/html/boost_propertytree/parsers.html#boost_propertytree.parsers.info_parser), hence the name, however there are minor changes and boost is not needed.

The structure is inspired by json/xml but with a syntax inspired by C, C++ etc.

This is a basic info file:

    key1 value1
    key2
    {
       key3 value3
       {
          key4 "value4 with spaces"
       }
       key5 value5
    }

Here is a more complicated file demonstrating all the info features:

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
	names [ cat dog duck ] // like {} but all keys are empty strings

Separators exist but are optional, the following pairs are equivalent

	key value
	key = value;
	key := value,
	key : value
	
and since there are optional line terminators, these are also equivalent (spaces added for clarity)

    key ""
	key = "" ;
	key = ;
	key ;
	
The parser is built using flex/bison and a good windows build is found at http://sourceforge.net/projects/winflexbison/files/?source=navbar


Todo:
=======

pretty printing
writing to file
example files
BNF documentation
verify newline and coloumn/character loading
replace the std::vector
util node functions
unicode \escape support
tinyxml like handle class
