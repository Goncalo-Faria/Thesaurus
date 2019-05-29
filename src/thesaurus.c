#include "thesaurus.h"
#include "relation.h"
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
    ths->relations = g_hash_table_new_full( hashRelation, equalRelation, unmkRelation ,g_hash_table_destroy );
    
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
void putRelation( Thesaurus saurus , Relation rel){
    if( !g_hash_table_contains(saurus->relations,rel) ){
        g_hash_table_insert(saurus->relations, clone(rel), newRelationSetfromHashTable() );
    }
}

Relation getRelation( Thesaurus saurus, const char* r){
    retunr mkRelation(r);
}

void putConcept( Thesaurus, Concept cpt){
    if( !g_hash_table_contains(saurus->concepts,cpt->name) ){
        g_hash_table_insert(saurus->concepts, strdup(cpt->name), clone(cpt));
    }
}

Concept getConcept( Thesaurus saurus, const char* r ){
    return clone((Concept)g_hash_table_lookup(saurus,r));
}


/*
associa um novo par de relações.
*/
void relate( Thesaurus saurus, Relation subclasse/*NT*/, Relation superclasse/*BT*/){

    if( !g_hash_table_contains(saurus->metarelations,superclasse) ){
        g_hash_table_insert(saurus->metarelations, clone(superclasse), g_hash_table_new( hashRelation, equalRelation) );
    }

    if( !g_hash_table_contains(saurus->metarelations,subclasse) ){
        g_hash_table_insert(saurus->metarelations, clone(subclasse), g_hash_table_new( hashRelation, equalRelation) );
    }

    GHashTable * rl = g_hash_table_lookup(saurus->metarelations,superclasse);
    Relation cpy = clone(subclasse);

    g_hash_table_insert(rl, cpy, cpy);
}

void associate(Thesaurus saurus, Concept source, Relation r, Concept associatee){
    link(source,r,associatee);
    RelationSet rs = g_hash_table_lookup(saurus->relations, r);
    GList * relationListing = g_hash_table_get_keys (rs);

    for( GList * cur = relationListing; cur ; cur = cur->next )
        link(associatee, (Relation)cur->data, source);
    
    g_list_free(relationlisting);
}
