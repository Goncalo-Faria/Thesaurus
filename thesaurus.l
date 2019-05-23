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
%option noyywrap
%%
\%language                  { return LANGDEC;/*declaring suported languages*/}
\%baselang                  { return BASELANGDEC;/*declaring base language*/}
\#[^\n]*\n                  { ;/*comment*/ }
NT                          { return NT;/*narrow term*/}
BT                          { return BT;/*broader term*/}
SN                          { return SN;/*scope name*/}
[^,\n]+/[\n,]               {
                                yylval.string = strtolower(strdup(yytext)); 
                                return PHRASE;
                                /*phrase*/
                            }
,                           { return SEPARATOR; }
[A-Z]+					    {
                                yylval.string = strdup(yytext); 
                                return LANG;
                                /*language identifier*/
                            }
.|\n 						{ ; /* garbage*/}
%%