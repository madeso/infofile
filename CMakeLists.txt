cmake_minimum_required (VERSION 2.6)
project (infofile)

include_directories(${infofile_SOURCE_DIR})

add_executable(infofile-test test.cpp infofile/infofile.cpp infofile/infofile.h infofile/configuration.h infofile/lexer.cpp infofile/parser.cpp)