find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser parser.y ${infofile_SOURCE_DIR}/infofile/parser.cpp)
FLEX_TARGET(MyScanner lexer.l  ${infofile_SOURCE_DIR}/infofile/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(infofile-test lexer.l parser.y test.cpp infofile.cpp infofile.h configuration.h ${BISON_MyParser_OUTPUTS} ${FLEX_MyScanner_OUTPUTS})