#include "include/concept.h"
#include "include/relation.h"
#include <glib.h>

typedef struct concept{
    char * name;
    GHashTable * related; /* Map<Relation, Set<Concept> >*/
    GHashTable * translation; /* Map<lang, translation> */
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
    cpt->translation = g_hash_table_new_full(
        g_str_hash,
        g_str_equal,
        free,
        free
    );

    return cpt;
}

void showConcept( Concept cpt ){
    
    GList * rwrlist = g_hash_table_get_keys(cpt->related);
    GList * rwcslist = g_hash_table_get_values(cpt->related);

    GList * rlist = rwrlist;
    GList * cslist = rwcslist;

    printf("%s ->\n", cpt->name);

    GList * rwtransk = g_hash_table_get_keys(cpt->translation);
    GList * rwtransv = g_hash_table_get_values(cpt->translation);
    GList * transk = rwtransk;
    GList * transv = rwtransv;

    while( transk && transv ){
        printf("\t%s =====> %s \n", (char*)transk->data, (char*)transv->data);

        transv = transv->next;
        transk = transk->next;
    }

    g_list_free(rwtransk);
    g_list_free(rwtransv);

    while( rlist && cslist ){
        Relation r = (Relation)rlist->data;
        ConceptSet cs = (ConceptSet)cslist->data;

        GList * clist = g_hash_table_get_keys(cs);
        showRelation(r);
        for( GList* cur = clist; cur; cur = cur->next ){
            Concept innercn = (Concept)cur->data;    
            printf("\t  : %s \n",innercn->name);
        }

        g_list_free(clist);
        rlist = rlist->next;
        cslist = cslist->next;
    }

    g_list_free(rwrlist);
    g_list_free(rwcslist);

}

void translation( Concept cpt, const char * lang, const char * translated ){
    g_hash_table_insert(
        cpt->translation, 
        strdup(lang), 
        strdup(translated)
    );
}

int isRelatedBy( Concept cpt, Relation edge){
    return g_hash_table_contains(cpt->related, edge);
}

void fillConceptHistogram(Concept cpt, Relation edge, GHashTable * conceptHist){
    if( g_hash_table_contains(cpt->related,edge) ){
        ConceptSet cs = g_hash_table_lookup(cpt->related,edge);
        GList* cskeys = g_hash_table_get_keys(cs);
        
        for( GList* cur = cskeys; cur; cur = cur->next){
            Concept tmpcpt = (Concept)cur->data;
            if( g_hash_table_contains(conceptHist,tmpcpt) ){
                int *count = g_hash_table_lookup(conceptHist,tmpcpt);
                *count = *count + 1;
            }else{

                int * count = (int*)malloc(sizeof(int));
                *count = 1;
                g_hash_table_insert(
                    conceptHist, 
                    tmpcpt, 
                    count);

            }
        } 
        g_list_free(cskeys);
    }
}

/*
    Identifica os arcos(relações) que ligam os dois vértices (conceitos).
*/
GList * identifyRelations(Concept origin, Concept dest){
    GList* rslist = g_hash_table_get_keys(origin->related);
    GList* solution = NULL;
    
    for( GList* cur = rslist; cur; cur = cur->next){
        if( g_hash_table_contains(
                (ConceptSet)g_hash_table_lookup(
                    origin->related,
                    (Relation)cur->data
                ),
                dest
            ) 
        )
            solution = g_list_prepend(solution, cur->data);
    }

    g_list_free(rslist);

    return solution;
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
    g_hash_table_destroy(cpt->translation);
    free(cpt);
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
