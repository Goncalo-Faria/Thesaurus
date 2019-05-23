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
%token <string> TERM
%token <string> LANG


%type <string> Thesaurus Concepts Specs Spec Langs Concept ConceptList Property Properties Phrase Terms Str
%%

Thesaurus 	: Specs Concepts Remaining 	 	{$$ = strdup(" ");}
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

Remaining 	: Remaining NEWLINE
			| NEWLINE
			;

Concept		: NEWLINE WORD Properties		{$$ = strdup(" ");}
			| NEWLINE WORD					{$$ = strdup(" ");}
			;

Properties 	: Properties Property			{$$ = strdup(" ");}
			| Property						{$$ = strdup(" ");}
			;

Property	: LANG Phrase					{$$ = strdup(" ");}
			| NT ConceptList				{$$ = strdup(" ");}
			| BT ConceptList				{$$ = strdup(" ");}
			| SN Terms						{$$ = strdup(" ");}
			;

ConceptList : ConceptList SEPARATOR Phrase	{$$ = strdup(" ");}
			| Phrase					    {$$ = strdup(" ");}
			;

Phrase		: Phrase WORD					{$$ = strdup(" ");}
			| WORD							{$$ = strdup(" ");}
			;

Terms		: Terms Str						{$$ = strdup(" ");}
			| Str							{$$ = strdup(" ");}
			;

Str 		: WORD							{$$ = strdup(" ");}
			| TERM							{$$ = strdup(" ");}
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
