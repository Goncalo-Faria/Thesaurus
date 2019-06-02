#include "include/ontology.h"
#include "include/relation.h"
#include <glib.h>

typedef struct ontology{
    char * baselanguage;
    GHashTable * suportedLanguages; /* Set<char *> */
    GHashTable * concepts; /* Map<char*, Concept>*/
    GHashTable * relations; /* Map< Relation, Set<Relation> >*/
} *Ontology;

void setBaseLanguage(Ontology saurus, const char * lang){
    saurus->baselanguage = strdup(lang);
}

void addLanguage(Ontology saurus, const char* lang){
    char * cpy = strdup(lang);
    g_hash_table_insert(saurus->suportedLanguages, cpy, cpy);
}

Ontology mkOntology(){
    Ontology ths = (Ontology)malloc(sizeof(struct ontology));
    ths->concepts = g_hash_table_new_full(
        g_str_hash, 
        g_str_equal, 
        free, 
        unmkConcept);
    ths->relations = g_hash_table_new_full( 
        hashRelation, 
        equalRelation, 
        unmkRelation,
        g_hash_table_destroy 
    );

    ths->suportedLanguages = g_hash_table_new_full(
        g_str_hash, 
        g_str_equal,
        free,
        NULL 
    );

    return ths;
}

void unmkOntology( Ontology saurus ){
    g_hash_table_destroy(saurus->concepts);
    g_hash_table_destroy(saurus->relations);
    g_hash_table_destroy(saurus->suportedLanguages);
    free(saurus->baselanguage);
    free(saurus);
}
/*
void addLanguage( Ontology saurus, char * lang ){
    GString slan = g_string_new( lang );
    g_hash_table_insert(saurus->languages, slan, NULL);
}
*/

void showOntology(Ontology saurus){

    int number_of_concepts = g_hash_table_size(saurus->concepts);
    int number_of_relations = g_hash_table_size(saurus->relations);

    printf("baselanguage : %s\n", saurus->baselanguage);
    
    GList* langs = g_hash_table_get_values(saurus->suportedLanguages);

    printf("\tsupported languages");
    for(GList * cur= langs; cur; cur = cur->next )
        printf(" %s",(char*)cur->data);

    printf("\n");
    g_list_free(langs); 

    GList * concepts = g_hash_table_get_values(saurus->concepts);

    for(GList * cur = concepts; cur; cur = cur->next)
        showConcept( (Concept)cur->data );

    g_list_free(concepts);

    /*
    GList * relationSet = g_hash_table_get_values(saurus->concepts);

    for(GList* cur = relationSet; cur ; cur = cur->next ){
        RelationSet rs = (RelationSet)cur->data;
        GList * lrs = g_hash_table_get_keys(rs);
        
        for(GList* innercur = lrs; innercur; innercur = innercur->next){
            showRelation((Relation)innercur->data);
        }

        g_list_free(lrs),
    }

    g_list_free(relationSet);
        
    printf(" num of concepts : %d, num of relations : %d \n",number_of_concepts, number_of_relations);
    */
}

/*
adciona uma nova entrada ao conjunto de meta relações.
*/
Relation getRelation( Ontology saurus, const char* relationname ){
    
    Relation result = mkRelation(relationname);

    if( !g_hash_table_contains(saurus->relations, result) &&
                !g_hash_table_contains(saurus->suportedLanguages, relationname) ){
        g_hash_table_insert(
            saurus->relations, 
            mkRelation(relationname), 
            newRelationSetfromHashTable() 
        );
    }
    return result;
}

Concept getConcept( Ontology saurus, const char* conceptname ){

    if( ! g_hash_table_contains(saurus->concepts, conceptname) ){
        g_hash_table_insert(
            saurus->concepts,
            strdup(conceptname), 
            mkConcept(conceptname)
        );
    }
  
    return (Concept)g_hash_table_lookup(saurus->concepts,conceptname);
}

void relateRaw(Ontology saurus, const char* subclasse, const char* superclasse ){
    Relation a = getRelation( saurus, subclasse);
    Relation b = getRelation( saurus, superclasse);

    relate( saurus, a, b);
}

/*
associa um novo par de relações.
*/

void relate( Ontology saurus, Relation subclasse/*NT - tem de ser avisado */, Relation superclasse/*BT*/){

    if( !g_hash_table_contains(saurus->relations,superclasse) ){
        g_hash_table_insert(saurus->relations, clone(superclasse), g_hash_table_new( hashRelation, equalRelation) );
    }

    if( !g_hash_table_contains(saurus->relations,subclasse) ){
        g_hash_table_insert(saurus->relations, clone(subclasse), g_hash_table_new( hashRelation, equalRelation) );
    }

    GHashTable * rl = g_hash_table_lookup(saurus->relations, superclasse);

    Relation cpy = clone(subclasse);
    g_hash_table_insert(rl, cpy, cpy);
}

void associate(Ontology saurus, Concept source, const char * relation, const char * assoc){
    
    if( g_hash_table_contains(saurus->suportedLanguages, relation) ){

        translation( source, relation, assoc);

    }else{

        Relation r = getRelation(saurus, relation);
        Concept associatee = getConcept(saurus, assoc);

        linkConcepts( source, r, associatee);
        RelationSet rs = g_hash_table_lookup( saurus->relations, r);
        GList * relationlisting = g_hash_table_get_keys(rs);

        for( GList * cur = relationlisting; cur ; cur = cur->next )
            linkConcepts(associatee, (Relation)cur->data, source);
    
        g_list_free(relationlisting);
    }

}
