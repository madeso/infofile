set(src
    infofile/infofile.cc infofile/infofile.h
    infofile/parser.cc infofile/parser.h
    infofile/lexer.cc infofile/lexer.h
    infofile/file.cc infofile/file.h
    infofile/node.cc infofile/node.h
    infofile/reader.cc infofile/reader.h
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
    ../external/catch/main.cc
)
add_executable(test ${src_test})
target_link_libraries(test
    PUBLIC catch infofile catchy
    PRIVATE project_options project_warnings
)

