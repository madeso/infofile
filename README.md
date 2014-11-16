I got tired of different parsing libraries, weird specifications and strange file formats, so I made my own. This is based on the [boost info file](http://www.boost.org/doc/libs/1_56_0/doc/html/boost_propertytree/parsers.html#boost_propertytree.parsers.info_parser), hence the name, however there are minor changes and boost is not needed.

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
    key1 value1   // Another comment
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
	
For windows, a good build of flex/bison is found at http://sourceforge.net/projects/winflexbison/files/?source=navbar