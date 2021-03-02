set(src
    infofile/infofile.cc infofile/infofile.h
    infofile/chars.cc infofile/chars.h
    infofile/parser.cc infofile/parser.h
    infofile/lexer.cc infofile/lexer.h
    infofile/file.cc infofile/file.h
    infofile/node.cc infofile/node.h
    infofile/reader.cc infofile/reader.h
    infofile/printstring.cc infofile/printstring.h
)

add_library(infofile STATIC ${src})
target_link_libraries(infofile
    PUBLIC fmt::fmt
    PRIVATE project_options project_warnings
)
target_include_directories(infofile
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

source_group("" FILES ${src})

set(src_test
    infofile/infofile.test.cc
    infofile/lexer.test.cc
    infofile/printstring.test.cc
    ../external/catch_main.cc
)
add_executable(tests ${src_test})
target_link_libraries(tests
    PUBLIC catch infofile catchy fmt::fmt
    PRIVATE project_options project_warnings
)

catch_discover_tests(tests)
