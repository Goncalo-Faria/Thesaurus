%{
#include "y.tab.h"	
#include "string.h"
#include "ctype.h"

char* strtolower( char * str ){
    for( char * cur = str; *cur != '\0'; cur++)
        *cur = tolower(*cur);
    return str;
}

%}
%x NONDEFINED
%option noyywrap
%%
\%language                  { return LANGDEC;/*declaring suported languages*/}
\%baselang                  { return BASELANGDEC;/*declaring base language*/}
\%inv                       { return RELATE; }
\%title                     { return TITLE; }
\#[^\n]*                    { ; /*comment*/}
,                           { return SEPARATOR;}
[A-Z][A-Z]+				    {   
                                yylval.string = strdup(yytext); 
                                return LANG;
                                /*language identifier*/
                            }
[a-zA-Z()+/\-&$!?]+         {   
                                yylval.string = strdup(yytext); 
                                return WORD;
                                /*phrase*/
                            }
\%                          { BEGIN NONDEFINED; }
<NONDEFINED>[^\n]+/\n       { BEGIN INITIAL; }
\n\n+                       { return NEWLINE; }
" "|\n 						{ ; /* garbage*/  }
%%