%option noyywrap
%option nodefault

%option debug

%x STATE_PROPERTY
%x STATE_BYTE_STRING

%{
#include <stdio.h>
#include <string>
#include "Driver.h"

using token = yy::parser::token;

%}

U32_DEC     ([0-9]+)
U32_HEX	    (0[xX]([a-fA-F0-9]){1,8}|[0-9]+)
U32         ({U32_DEC}|{U32_HEX}) 
PATH	    (([a-zA-Z0-9,._+-/]+[^/])(@({U32}))?)
BLANK	    ([ \t\r])
NEWLINE	    ([\n])

%%

";"     { return token::TERMINATOR; }
"{"     { return token::NODE_BEGIN; }
"}"     { return token::NODE_END; }

"/"dts-v1"/" {
    yylval->build<std::string>(yytext);
    return token::DIRECTIVE_DTS_VERSION;
}

([a-zA-Z0-9,._+-]+(@({U32}))?)|"/" {
    yylval->build<std::string>(yytext);
    return token::NODE_NAME;
}

[a-zA-Z0-9_]+{BLANK}*: {
    int i = 0;
    while (yytext[i] != ' ' && yytext[i] != ':') i++;
    yytext[i] = '\0';
    yylval->build<std::string>(yytext);
    return token::LABEL;
}

("&"[a-zA-Z0-9_]+)|("&{"{BLANK}*{PATH}{BLANK}*"}") {
    yylval->build<std::string>(yytext + 1);
    return token::PROP_VALUE_REF;
}

[a-zA-Z0-9,"."_+?#-]+{BLANK}*= {
    int i = 0;
    while (yytext[i] != ' ' && yytext[i] != '=') i++;
    yytext[i] = '\0';
    yylval->build<std::string>(yytext);

    BEGIN STATE_PROPERTY;
    return token::PROP_NAME;
}

<STATE_PROPERTY>[a-zA-Z0-9_]+{BLANK}*: {
    int i = 0;
    while (yytext[i] != ' ' && yytext[i] != ':') i++;
    yytext[i] = '\0';
    yylval->build<std::string>(yytext);
    return token::LABEL;
}

<STATE_PROPERTY>{U32} {
    yylval->build<uint32_t>((uint32_t)std::stoul(yytext, nullptr, 0));
    return token::PROP_VALUE_U32;
}

<STATE_PROPERTY>("&"[a-zA-Z0-9_]+)|("&""{"{BLANK}*{PATH}{BLANK}*"}") {
    yylval->build<std::string>(yytext + 1);
    return token::PROP_VALUE_REF;
}

<STATE_PROPERTY>"<"{BLANK}* {
    return token::PROP_ARRAY_BEGIN;
}

<STATE_PROPERTY>{BLANK}*">" {
    return token::PROP_ARRAY_END;
}

<STATE_PROPERTY>"["{BLANK}* {
    BEGIN STATE_BYTE_STRING;
    return token::PROP_BYTE_STR_BEGIN;
}

<STATE_PROPERTY>"," {
    return token::COMMA;
}

<STATE_PROPERTY>\"[^\"]+\" {
    yytext[strlen(yytext) - 1] = '\0';
    yylval->build<std::string>(yytext + 1);
    return token::PROP_VALUE_STR;
}

<STATE_PROPERTY>";" {
    BEGIN INITIAL;
    return token::TERMINATOR;
}

<STATE_BYTE_STRING>[a-zA-Z0-9_]+{BLANK}*: {
    int i = 0;
    while (yytext[i] != ' ' && yytext[i] != ':') i++;
    yytext[i] = '\0';
    yylval->build<std::string>(yytext);
    return token::LABEL;
}

<STATE_BYTE_STRING>[a-fA-F0-9]+{2} {
    uint8_t byte = (uint8_t)std::stoul(yytext, nullptr, 16);
    yylval->build<uint8_t>(byte);
    return token::PROP_VALUE_BYTE;
}

<STATE_BYTE_STRING>[a-fA-F0-9]+ {
    yylval->build<std::string>(yytext);
    return token::PROP_VALUE_BYTE_NO_SPACE;
}

<STATE_BYTE_STRING>{BLANK}*"]" {
    BEGIN STATE_PROPERTY;
    return token::PROP_BYTE_STR_END;
}

{BLANK}|{NEWLINE}                           ;
<STATE_PROPERTY>{BLANK}|{NEWLINE}           ;
<STATE_BYTE_STRING>{BLANK}|{NEWLINE}        ;

. { 
    fprintf(stderr, "Unexpected character: %s\n", yytext); 
    return token::YYerror;
}

<STATE_PROPERTY>. { 
    fprintf(stderr, "Unexpected character: %s\n", yytext); 
    return token::YYerror;
}

<STATE_BYTE_STRING>. { 
    fprintf(stderr, "Unexpected character: %s\n", yytext); 
    return token::YYerror;
}

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
