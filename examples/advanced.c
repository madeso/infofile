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