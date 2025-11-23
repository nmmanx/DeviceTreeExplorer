%require "3.2"
%language "c++"

%code requires {
#include <string>
#include <vector>
#include <iostream>
#include <memory>

namespace dtparser {
    class IDriver;
    struct Element;
}

typedef std::shared_ptr<const dtparser::Element> ElementRef;
}

%parse-param {dtparser::IDriver *driver}

%locations

%define parse.trace
%define parse.error verbose

%code {
#include <Driver.h>
}

%define api.value.type variant

%token ROOT
%token TERMINATOR
%token NODE_BEGIN
%token NODE_END
%token PROP_VALUE_BEGIN
%token PROP_VALUE_END

%token <std::string> DIRECTIVE_DTS_VERSION
%token <std::string> DIRECTIVE_INCLUDE

%token <std::string> NODE_NAME
%token <std::string> NODE_LABEL
%token <std::string> PROP_NAME
%token <std::string> PROP_ENCODED_ARRAY_ELEMENT
%token <std::string> PROP_STR_VALUE

%nterm <ElementRef> directive
%nterm <ElementRef> node
%nterm <std::vector<ElementRef>> node_nody
%nterm <ElementRef> property

%nterm <std::vector<std::string>> property_values

%%

%start toplevel;

toplevel:
    %empty {}
    | toplevel directive TERMINATOR {
        driver->buildHierarchy($2, nullptr);
    } |
    toplevel node TERMINATOR {
        driver->buildHierarchy($2, nullptr);
    }
    ;

directive:
    DIRECTIVE_DTS_VERSION { 
        $$ = driver->newDirective($1);
    }
    ;

node:
    NODE_LABEL NODE_NAME NODE_BEGIN node_nody NODE_END {
        $$ = driver->newNode($2, $1);
        driver->buildHierarchy($$, $4);
    } | 
    NODE_NAME NODE_BEGIN node_nody NODE_END {
        $$ = driver->newNode($1);
        driver->buildHierarchy($$, $3);
    }
    ;

node_nody:
    %empty {} |
    node_nody property TERMINATOR {
        $$.push_back($2);
    } |
    node_nody node TERMINATOR {
        $$.push_back($2);
    }
    ;

property:
    PROP_NAME PROP_VALUE_BEGIN property_values PROP_VALUE_END {
        $$ = driver->newProperty($1, $3);
    } |
    PROP_NAME PROP_STR_VALUE {
        $$ = driver->newProperty($1, { $2 });
    }
    ;

property_values:
    PROP_ENCODED_ARRAY_ELEMENT { 
        $$.push_back($1);
    } |
    property_values PROP_ENCODED_ARRAY_ELEMENT {
        for (auto val: $1) {
            $$.push_back(val);
        }
    }
    ;

%%

void yy::parser::error(const location_type& l, const std::string& m) {
    std::cerr << l << ": " << m << std::endl;
}
