# Generate the parser and header
add_custom_target(gen_parser
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/gen/dtparser.cpp
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/gen
    COMMAND ${bison} ${CMAKE_CURRENT_SOURCE_DIR}/dtparser.y -vd -Wcounterexamples
        -o ${CMAKE_CURRENT_BINARY_DIR}/gen/dtparser.cpp -H
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/dtparser.y
)

# Generate the lexer
add_custom_target(gen_lexer
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/gen/dtlexer.cpp
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/gen
    COMMAND ${flex} -o ${CMAKE_CURRENT_BINARY_DIR}/gen/dtlexer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dtlexer.l
    DEPENDS gen_parser ${CMAKE_CURRENT_SOURCE_DIR}/dtlexer.l
)

add_dependencies(gen_parser flex_bison_tools)
add_dependencies(gen_lexer flex_bison_tools)

file(GLOB PARSER_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/gen/*.cpp
)

add_library(dtparser SHARED
    DeviceTree.cpp
    DeviceTreeSource.cpp
    Driver.cpp
    Property.cpp
    Node.cpp
    Element.cpp
    Reference.cpp
    Label.cpp
    ${PARSER_SOURCES}
)

add_dependencies(dtparser gen_lexer)

target_include_directories(dtparser PRIVATE include/private)
target_include_directories(dtparser PUBLIC include/public)
target_include_directories(dtparser PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/gen)
