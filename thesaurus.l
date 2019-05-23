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
\%language                  { printf("%s ",yytext); return LANGDEC;/*declaring suported languages*/}
\%baselang                  { printf("%s ",yytext); return BASELANGDEC;/*declaring base language*/}
\#[^\n#]*\n                 { printf("%s ",yytext); /*comment*/}
NT                          { printf("%s ",yytext); return NT;/*narrow term*/}
BT                          { printf("%s ",yytext); return BT;/*broader term*/}
SN                          { printf("%s ",yytext); return SN;/*scope name*/}
,                           { printf("%s ",yytext); return SEPARATOR;}
[A-Z][A-Z]				    {   
                                printf("%s ",yytext);
                                yylval.string = strdup(yytext); 
                                return LANG;
                                /*language identifier*/
                            }
[a-zA-Z]+                   {   
                                printf("%s\n",yytext);
                                yylval.string = strtolower(strdup(yytext)); 
                                return WORD;
                                /*phrase*/
                            }
\%                          { BEGIN NONDEFINED; }
<NONDEFINED>.               { ; }
<NONDEFINED>\n              { BEGIN INITIAL; printf("&\n"); }
\n                          { return NEWLINE; }
. 						    { ; /* garbage*/}
%%