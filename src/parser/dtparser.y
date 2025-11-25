%require "3.2"
%language "c++"

%code requires {
#include <string>
#include <vector>
#include <iostream>
#include <memory>

namespace dtparser {
    class IDriver;
}
}

%parse-param {dtparser::IDriver *driver}

%locations

%define parse.trace
%define parse.error verbose

%code {
#include <Driver.h>
using namespace dtparser;
}

%define api.value.type variant

%token ROOT
%token TERMINATOR
%token COMMA
%token NODE_BEGIN
%token NODE_END
%token PROP_ARRAY_BEGIN
%token PROP_ARRAY_END
%token PROP_BYTE_STR_BEGIN
%token PROP_BYTE_STR_END

%token <std::string> DIRECTIVE_DTS_VERSION
%token <std::string> DIRECTIVE_INCLUDE

%token <std::string> LABEL
%token <std::string> NODE_NAME
%token <std::string> PROP_NAME

%token <std::string> PROP_VALUE_REF
%token <uint32_t> PROP_VALUE_U32
%token <std::string> PROP_VALUE_STR
%token <uint8_t> PROP_VALUE_BYTE
%token <std::string> PROP_VALUE_BYTE_NO_SPACE

%nterm <std::string> label

%nterm <uint32_t> directive

%nterm <uint32_t> node
%nterm <std::vector<uint32_t>> node_body

%nterm <uint32_t> property
%nterm <std::vector<uint32_t>> property_component
%nterm <std::vector<uint32_t>> property_value
%nterm <std::vector<uint32_t>> property_cell
%nterm <std::vector<uint32_t>> property_byte_string

%%

%start toplevel;

toplevel:
    %empty {}
    | toplevel directive TERMINATOR {
        driver->buildHierarchy($2, 0);
    } |
    toplevel node TERMINATOR {
        driver->buildHierarchy($2, 0);
    }
    ;

directive:
    DIRECTIVE_DTS_VERSION { 
        $$ = driver->newDirective($1, {}, @$);
    }
    ;

label:
    %empty { $$ = ""; } |
    LABEL {
        $$ = $1;
    }
    ;

node:
    label NODE_NAME NODE_BEGIN node_body NODE_END {
        $$ = driver->newNode($2, @$);
        driver->addLabel($1, $$, @1);
        driver->buildHierarchy($$, $4);
    }
    ;

node_body:
    %empty {} |
    node_body property TERMINATOR {
        $$.push_back($2);
    } |
    node_body node TERMINATOR {
        $$.push_back($2);
    }
    ;

property:
    label PROP_NAME property_component  {
        $$ = driver->newProperty($2, @$);
        driver->addLabel($1, $$, @1);
        driver->buildHierarchy($$, $3);
    } |
    label PROP_NAME {
        $$ = driver->newProperty($2, @$);
        driver->addLabel($1, $$, @1);
    }
    ;

property_component:
    label property_value label {
        $$.insert($$.end(), $2.begin(), $2.end());
    } |
    property_component COMMA label property_value label {
        $$.insert($$.end(), $4.begin(), $4.end());
    }
    ;

property_value:
    PROP_VALUE_STR {
        $$.push_back(driver->newPropertyValue($1, @1));
    } |
    PROP_VALUE_U32 {
        $$.push_back(driver->newPropertyValue($1, @1));
    } |
    PROP_VALUE_REF {
        Reference ref = driver->makeReference($1, @1);
        $$.push_back(driver->newPropertyValue(ref, @1));
    } |
    PROP_ARRAY_BEGIN property_cell PROP_ARRAY_END {
        $$.insert($$.end(), $2.begin(), $2.end());
    } |
    PROP_BYTE_STR_BEGIN property_byte_string PROP_BYTE_STR_END {
        $$.insert($$.end(), $2.begin(), $2.end());
    }
    ;

property_cell:
    %empty {} |
    property_cell LABEL {
        ValueLabel label = ValueLabel { 
            name: $2,
            loc: @2
        };
        $$.push_back(driver->newPropertyValue(label, @2));
    } |
    property_cell PROP_VALUE_U32 {
        $$.push_back(driver->newPropertyValue($2, @2));
    } |
    property_cell PROP_VALUE_REF {
        Reference ref = driver->makeReference($2, @2);
        $$.push_back(driver->newPropertyValue(ref, @2));
    }
    ;

property_byte_string:
    %empty {} |
    property_byte_string LABEL {
        ValueLabel label = ValueLabel { 
            name: $2,
            loc: @2
        };
        $$.push_back(driver->newPropertyValue(label, @2));
    } |
    property_byte_string PROP_VALUE_BYTE {
        $$.push_back(driver->newPropertyValue($2, @2));
    } |
    PROP_VALUE_BYTE_NO_SPACE {
        // TODO: make bytes
    }
    ;

%%

void yy::parser::error(const location_type& l, const std::string& m) {
    std::cerr << l << ": " << m << std::endl;
}
