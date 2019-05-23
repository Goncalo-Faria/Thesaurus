%{
#define _GNU_SOURCE //asprintf
#include <stdio.h>
#include <string.h>

void yyerror(char* s);

%}
%union {char* string;}

/*sections*/
%token LANGDEC
%token BASELANGDEC

%token NT BT SN
%token SEPARATOR
%token NEWLINE

%token <string> WORD
%token <string> LANG


%type <string> Thesaurus Concepts Specs Spec Langs Concept ConceptList Property Properties
%%

Thesaurus 	: Specs Concepts  				{$$ = strdup(" ");}
			;

Specs		: Specs Spec					{$$ = strdup(" ");}
			| Spec							{$$ = strdup(" ");}
			;

Spec 		: LANGDEC Langs					{$$ = strdup(" ");}
			| BASELANGDEC LANG				{$$ = strdup(" ");}
			;
	
Langs		: Langs LANG					{$$ = strdup(" ");}
			| LANG							{$$ = strdup(" ");}
			;

Concepts 	: Concepts Concept				{$$ = strdup(" ");}
			| Concept						{$$ = strdup(" ");}
			;

Concept		: NEWLINE WORD Properties		{$$ = strdup(" ");}
			| WORD							{$$ = strdup(" ");}
			;

Properties 	: Properties Property			{$$ = strdup(" ");}
			| Property						{$$ = strdup(" ");}
			;

Property	: LANG WORD						{$$ = strdup(" ");}
			| NT ConceptList				{$$ = strdup(" ");}
			| BT ConceptList				{$$ = strdup(" ");}
			| SN Phrase						{$$ = strdup(" ");}
			;

Phrase		: Phrase WORD
			| WORD
			;

ConceptList : ConceptList SEPARATOR WORD	{$$ = strdup(" ");}
			| WORD							{$$ = strdup(" ");}
			;
%%

#include "lex.yy.c"

void yyerror(char* s){
	printf("Erro sintático %s\n",s); exit(1);
}

int main(){
	yyparse();
	return(0);
}
