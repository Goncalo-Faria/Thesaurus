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

%token <string> PHRASE
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

Concept		: PHRASE Properties				{$$ = strdup(" ");}
			| PHRASE						{$$ = strdup(" ");}
			;

Properties 	: Properties Property			{$$ = strdup(" ");}
			| Property						{$$ = strdup(" ");}
			;

Property	: LANG PHRASE					{$$ = strdup(" ");}
			| NT ConceptList				{$$ = strdup(" ");}
			| BT ConceptList				{$$ = strdup(" ");}
			| SN PHRASE						{$$ = strdup(" ");}
			;

ConceptList : ConceptList SEPARATOR PHRASE	{$$ = strdup(" ");}
			| PHRASE						{$$ = strdup(" ");}
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
