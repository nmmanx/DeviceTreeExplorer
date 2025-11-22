if (WIN32)
file(GLOB FLEX_BISON_HINTS "${CMAKE_CURRENT_SOURCE_DIR}/win_flex_bison-*")
find_program(flex
    NAME win_flex.exe
    HINTS ${FLEX_BISON_HINTS} REQUIRED)

find_program(bison
    NAME win_bison.exe
    HINTS ${FLEX_BISON_HINTS} REQUIRED)
elseif(UNIX)
find_program(flex REQUIRED)
find_program(bison REQUIRED)
endif()

execute_process(
    COMMAND ${flex} --version
    OUTPUT_VARIABLE FLEX_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX MATCH "^[^\n]*" FLEX_VERSION "${FLEX_VERSION}")
message(STATUS "flex: ${FLEX_VERSION}")

execute_process(
    COMMAND ${bison} --version
    OUTPUT_VARIABLE BISON_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(REGEX MATCH "^[^\n]*" BISON_VERSION "${BISON_VERSION}")
message(STATUS "bison: ${BISON_VERSION}")

add_custom_target(flex_bison_tools
    DEPENDS ${flex} ${bison}
)