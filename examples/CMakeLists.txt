set(src
    commandline.cc
)

add_executable(commandline ${src})
target_link_libraries(
    commandline
    PUBLIC infofile
    PRIVATE project_options project_warnings
)

source_group("" FILES ${src})
