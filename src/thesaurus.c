#include "include/thesaurus.h"
#include "include/relation.h"
#include <glib.h>

typedef struct thesaurus{
    //char * baselanguage;
    //GHashTable * translation; < language, translation >
    GHashTable * concepts; /* Map<char* Concept>*/
    GHashTable * relations; /* Map< Relation, Set<Relation> >*/
} *Thesaurus;

Thesaurus mkThesaurus(){
    Thesaurus ths = (Thesaurus)malloc(sizeof(struct thesaurus));
    ths->concepts = g_hash_table_new_full( g_str_hash, g_str_equal, free, unmkConcept);
    ths->relations = g_hash_table_new_full( 
        hashRelation, 
        equalRelation, 
        unmkRelation,
        g_hash_table_destroy 
    );
    
    return ths;
}

/*
void addLanguage( Thesaurus saurus, char * lang ){
    GString slan = g_string_new( lang );
    g_hash_table_insert(saurus->languages, slan, NULL);
}
*/

/*
adciona uma nova entrada ao conjunto de meta relações.
*/
Relation getRelation( Thesaurus saurus, const char* relationname ){
    
    Relation result = mkRelation(relationname);

    if( !g_hash_table_contains(saurus->relations, result) ){
        g_hash_table_insert(
            saurus->relations, 
            result, 
            newRelationSetfromHashTable() 
        );
    }

    return result;
}

Concept getConcept( Thesaurus saurus, const char* conceptname ){

    if( ! g_hash_table_contains(saurus->concepts, conceptname) ){
        g_hash_table_insert(
            saurus->concepts,
            strdup(conceptname), 
            mkConcept(conceptname)
        );
    }
  
    return (Concept)g_hash_table_lookup(saurus->concepts,conceptname);
}

void relateRaw(Thesaurus saurus, const char* subclasse, const char* superclasse ){
    Relation a = getRelation( saurus, subclasse);
    Relation b = getRelation( saurus, superclasse);

    relate( saurus, a, b);
}

/*
associa um novo par de relações.
*/

void relate( Thesaurus saurus, Relation subclasse/*NT - tem de ser avisado */, Relation superclasse/*BT*/){

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

void associate(Thesaurus saurus, Concept source, Relation r, Concept associatee){
    
    linkConcepts( source, r, associatee);
    RelationSet rs = g_hash_table_lookup( saurus->relations, r);
    GList * relationlisting = g_hash_table_get_keys(rs);

    for( GList * cur = relationlisting; cur ; cur = cur->next )
    linkConcepts(associatee, (Relation)cur->data, source);
    
    g_list_free(relationlisting);
}
