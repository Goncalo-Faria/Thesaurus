#include "include/concept.h"
#include "include/relation.h"
#include <glib.h>

typedef struct concept{
    char * name;
    GHashTable * related; /* Map<Relation, Set<Concept> >*/
} *Concept;

Concept mkConcept( const char * name ){

    Concept cpt = (Concept) malloc(sizeof(struct concept));
    cpt->name = strdup(name);
    cpt->related = g_hash_table_new_full(
        hashRelation,
        equalRelation,
        unmkRelation,
        g_hash_table_destroy 
    );

    return cpt;
}

unsigned int hashConcept( Concept key ){
    return g_str_hash(key->name);
}

int equalConcept( Concept a, Concept b){
    return g_str_equal(a->name, b->name);
}

ConceptSet newConceptSetfromHashTable(){
    return g_hash_table_new_full(
        hashConcept,
        equalConcept,
        NULL,
        NULL 
    );
}

void unmkConcept( Concept cpt ){
    free(cpt->name);
    g_hash_table_destroy(cpt->related);
}

/*
    adciona o conceito ao conjunto de conceitos de source associados por r.
*/
void linkConcepts(Concept source, Relation r, Concept associatee){
    if( !g_hash_table_contains(source->related, r) ){
        g_hash_table_insert(source->related, clone(r), newConceptSetfromHashTable() );
    }
    ConceptSet cs = g_hash_table_lookup(source->related, r);
    g_hash_table_insert(cs, associatee, associatee);
}
