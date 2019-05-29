#include <glib.h>
#include "relation.h"

typedef struct relation{
    char * name;
} * Relation;

/*      Relation        */

Relation mkRelation( const char * name ){
    
    Relation r = (Relation)malloc( sizeof(struct relation) );
    r->name = strdup(name);
    
    return r;
}

void unmkRelaiton( Relation r){
    free(r);
    free(r->name);
}

Relation clone( Relation rel ){
    return mkRelation(rel->name);
}

unsigned int hashRelation( Relation key ){
    return g_str_hash(key->name);
}

int equalRelation( Relation a, Relation b){
    return g_str_equal(a->name, b->name);
}

RelationSet newRelationSetfromHashTable(){
    return g_hash_table_new_full(
        hashRelation,
        equalRelation,
        unmkConcept,
        NULL 
    );
}
/*-------------------*/