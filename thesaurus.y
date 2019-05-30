%{
#define _GNU_SOURCE //asprintf
#include <stdio.h>
#include <string.h>
#include "src/include/thesaurus.h"
#include "src/include/concept.h"


typedef struct property{
	Relation r;
	GList * concepts;
} *Prop;

void yyerror(char* s);

Thesaurus saurus;

Prop mkProp(const char* relationname, GList* list){
	Prop p = (Prop)malloc(sizeof(struct property));
	p->r = getRelation(saurus,relationname);
	p->concepts = list;
	return p;
}

void unmkProp( Prop p ){
	g_list_free(p->concepts);
	free(p);
}

%}
%union {char* string; GList * list; Prop prop; }

/*sections*/
%token LANGDEC
%token BASELANGDEC
%token RELATE

%token SEPARATOR
%token NEWLINE

%token <string> WORD
%token <string> TERM
%token <string> LANG

%type <string> Terms
%type <list> ConceptList Properties 
%type <prop> Property
%%

Thesaurus 	: Start Specs Concepts Remaining { ; }
			;

Start 		:								 { saurus = mkThesaurus(); }
			;

Specs		: Specs Spec					 { ; }
			| 								 { ; }
			;

Spec 		: LANGDEC Langs					 { ; }
			| BASELANGDEC LANG				 { ; }
			| RELATE LANG LANG				 { 
				relateRaw(saurus,$2,$3); 	
			}
			;
	
Langs		: Langs LANG					{ ; }
			| LANG							{ ; }
			;

Concepts 	: Concepts Concept				{ ; }
			| Concept						{ ; }
			;

Remaining 	: Remaining NEWLINE
			| NEWLINE
			;

Concept		: NEWLINE WORD Properties		{
				Concept cpt = getConcept(saurus, $2);
												
				for(GList* cur = $3; cur; cur = cur->next){
					Prop tup = (Prop)cur->data;

					for(GList* innercur = tup->concepts; innercur; innercur= innercur->next )
						associate(saurus, cpt, tup->r, (Concept)innercur->data);

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
				$$ = g_list_prepend($1  , getConcept(saurus,$3));
			}
			| Terms					    	{
				$$ = g_list_prepend(NULL, getConcept(saurus,$1));
			}
			;

Terms		: Terms WORD					{
				asprintf(&$$,"%s%s",$1,$2);
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
