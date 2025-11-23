%option noyywrap
%option debug

%{
#include <stdio.h>
#include <string>
#include "dtparser/Driver.h"

using token = yy::parser::token;

%}

U32	(0[xX]([a-fA-F0-9]){1,8}|[0-9]+)
BLANK	([ \t\r])

%%

";"     { return token::TERMINATOR; }
"{"     { return token::NODE_BEGIN; }
"}"     { return token::NODE_END; }

"<"{BLANK}*  { return token::PROP_VALUE_BEGIN; }
{BLANK}*">"  { return token::PROP_VALUE_END; }

"/"dts-v1"/" {
    yylval->build<std::string>(yytext);
    return token::DIRECTIVE_DTS_VERSION;
}

{U32} {
    yylval->build<std::string>(yytext);
    return token::PROP_ENCODED_ARRAY_ELEMENT;
}

[a-zA-Z0-9,"."_+?#-]+{BLANK}*= {
    yylval->build<std::string>(yytext);
    return token::PROP_NAME;
}

([a-zA-Z0-9,\._+-]+(@{U32})*)|"/" {
    yylval->build<std::string>(yytext);
    return token::NODE_NAME;
}

[a-zA-Z0-9_]+: {
    yylval->build<std::string>(yytext);
    return token::NODE_LABEL;
}

\"[^\"]+\" {
    yylval->build<std::string>(yytext);
    return token::PROP_STR_VALUE;
}

{BLANK}    ;

%%

int scan_begin(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (f) {
        yyin = f;
    } else {
        fprintf(stderr, "Cannot open '%s': %d\n", filename, errno);
        exit(1);
    }
    return 0;
}

int scan_end()
{
    fclose(yyin);
    return 0;
}
