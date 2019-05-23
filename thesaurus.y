%{
#define _GNU_SOURCE //asprintf
#include <stdio.h>
#include <string.h>

char address;
int TABID[26];
int yylex();
int varcount = 0;
int jumpvar = 0;
#define UNDECLARED -1

void yyerror(char* s);

int lookup (char var){
	return TABID[var-'a'];
}

void declare (char var){
	if (TABID[var-'a'] != UNDECLARED) {
		yyerror("Variable alReady declared."); 
	}else {
		TABID[var -'a'] = varcount++;
	}
}

int getNewJump(){
	return ++jumpvar;
}

int getJump(){
	return jumpvar;
}

%}
%union {char* string;}

/*sections*/
%token LANGDEC
%token BASELANGDEC

%token NT BT SN
%token SEPARATOR

%token <string> PHRASE
%token <string> LANG


%type <string> Thesaurus Concepts Specs Spec Langs Concept ConceptList Properity Properties
%%

Thesaurus 	: Specs Concepts  				{return "";}
			;

Specs		: Specs Spec					{return "";}
			| Spec							{return "";}
			;

Spec 		: LANGDEC Langs					{return "";}
			| BASELANGDEC LANG				{return "";}
			;
	
Langs		: Langs LANG					{return "";}
			| LANG							{return "";}
			;

Concepts 	: Concepts Concept				{return "";}
			| Concept						{return "";}
			;

Concept		: PHRASE Properties				{return "";}
			;

Properties 	: Properties Properity			{return "";}
			|
			;

Property	: LANG PHRASE					{return "";}
			| NT ConceptList				{return "";}
			| BT ConceptList				{return "";}
			| SN PHRASE						{return "";}
			;

ConceptList : ConceptList SEPARATOR PHRASE	{return "";}
			| PHRASE						{return "";}
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
