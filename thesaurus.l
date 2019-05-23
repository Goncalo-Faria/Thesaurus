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
\#[^\n]*/\n                 { printf("commen:%s\n",yytext); /*comment*/}
NT                          { printf("nt:%s ",yytext); return NT;/*narrow term*/}
BT                          { printf("bt:%s ",yytext); return BT;/*broader term*/}
SN                          { printf("sn:%s ",yytext); return SN;/*scope name*/}
,                           { printf("%s ",yytext); return SEPARATOR;}
[A-Z][A-Z]				    {   
                                printf("lang:%s ",yytext);
                                yylval.string = strdup(yytext); 
                                return LANG;
                                /*language identifier*/
                            }
[a-zA-Z]+                   {   
                                printf("w:%s\n",yytext);
                                yylval.string = strtolower(strdup(yytext)); 
                                return WORD;
                                /*phrase*/
                            }
\%                          { BEGIN NONDEFINED; }
<NONDEFINED>.               { ; }
<NONDEFINED>\n              { BEGIN INITIAL; printf("&\n"); }
\n\n                        { return NEWLINE; }
.|\n 						{ ; /* garbage*/}
%%