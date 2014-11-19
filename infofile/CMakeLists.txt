set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake-modules")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

#set(GMOCK_CMAKE_DEBUG ON)
find_package(GoogleMock REQUIRED)

BISON_TARGET(MyParser parser.y ${infofile_SOURCE_DIR}/infofile/parser.cpp)
FLEX_TARGET(MyScanner lexer.l  ${infofile_SOURCE_DIR}/infofile/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(${GOOGLEMOCK_GTEST_INCLUDE_DIRS})
include_directories(${GOOGLEMOCK_INCLUDE_DIRS})

set(SRC infofile-test lexer.l parser.y test.cpp infofile.cpp infofile.h main.cpp configuration.h)

add_executable(${SRC} ${BISON_MyParser_OUTPUTS} ${FLEX_MyScanner_OUTPUTS})

source_group("" FILES ${SRC})
source_group("generated" FILES ${BISON_MyParser_OUTPUTS} ${FLEX_MyScanner_OUTPUTS})

target_link_libraries(infofile-test ${GOOGLEMOCK_LIBRARIES})
add_custom_command(TARGET infofile-test POST_BUILD
	COMMAND infofile-test
	WORKING_DIRECTORY ${${infofile_SOURCE_DIR}})