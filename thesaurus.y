%{
#define _GNU_SOURCE //asprintf
#include <stdio.h>
#include <string.h>
#include "src/include/ontology.h"
#include "src/include/concept.h"


typedef struct property{
	char * r;
	GList * concepts;/* list<char*>*/
} *Prop;

void yyerror(char* s);

Ontology saurus;

Prop mkProp(const char* relationname, GList* list){
	Prop p = (Prop)malloc(sizeof(struct property));
	p->r = strdup(relationname);
	p->concepts = list;
	return p;
}

void unmkProp( Prop p ){
	free(p->r);
	g_list_free_full(p->concepts,free);
	free(p);
}

%}
%union {char* string; GList * list; Prop prop; }

/*sections*/
%token LANGDEC
%token BASELANGDEC
%token RELATE
%token TITLE

%token SEPARATOR
%token NEWLINE

%token <string> WORD
%token <string> TERM
%token <string> LANG

%type <string> Terms
%type <list> ConceptList Properties 
%type <prop> Property
%%

Thesaurus 	: Start Specs Concepts Remaining { 
				showOntology(saurus); 
				unmkOntology(saurus); 
				}
			;

Start 		:								 { saurus = mkOntology(); }
			;

Specs		: Specs Spec					 { ; }
			| 								 { ; }
			;

Spec 		: LANGDEC Langs					 { ; }
			| BASELANGDEC LANG				 { setBaseLanguage(saurus,$2); }
			| RELATE LANG LANG				 { 
				relateRaw(saurus,$2,$3); 	
			}
			| TITLE Terms					{ addTitle(saurus,$2); }
			;
	
Langs		: Langs LANG					{ addLanguage(saurus, $2); }
			| LANG							{ addLanguage(saurus, $1); }
			;

Concepts 	: Concepts Concept				{ ; }
			| Concept						{ ; }
			;

Remaining 	: Remaining NEWLINE				{ ; }
			| 								{ ; }
			;

Concept		: NEWLINE WORD Properties		{
				Concept cpt = getConcept(saurus, $2);
												
				for(GList* cur = $3; cur; cur = cur->next){
					Prop tup = (Prop)cur->data;

					for(GList* innercur = tup->concepts; innercur; innercur= innercur->next )
						associate(saurus, cpt, tup->r, (char *)innercur->data);

					unmkProp(tup);
				}

				g_list_free($3);
			}
			| NEWLINE WORD					{ 
				getConcept(saurus, $2); 
			}
			;

Properties 	: Properties Property			{ 
				$$ = g_list_prepend($1  , $2); 
			}
			| Property						{ 
				$$ = g_list_prepend(NULL, $1); 
			}
			;

Property	: LANG ConceptList				{	
				$$ = mkProp($1, $2);
			}
			;

ConceptList : ConceptList SEPARATOR Terms	{
				$$ = g_list_prepend($1  , strdup($3));
			}
			| Terms					    	{
				$$ = g_list_prepend(NULL, strdup($1));
			}
			;

Terms		: Terms WORD					{
				asprintf(&$$,"%s %s",$1,$2);
			}
			| WORD							{
				$$ = strdup($1);
			}
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
