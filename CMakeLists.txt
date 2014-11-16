cmake_minimum_required (VERSION 2.6)
project (infofile)

include_directories(${infofile_SOURCE_DIR})

find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser infofile/parser.y ${infofile_SOURCE_DIR}/infofile/parser.cpp)
FLEX_TARGET(MyScanner infofile/lexer.l  ${infofile_SOURCE_DIR}/infofile/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(infofile-test infofile/lexer.l infofile/parser.y test.cpp infofile/infofile.cpp infofile/infofile.h infofile/configuration.h ${BISON_MyParser_OUTPUTS} ${FLEX_MyScanner_OUTPUTS})