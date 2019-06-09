#include "include/concept.h"
#include "include/relation.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

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

    GList *rwrlist = g_hash_table_get_keys(cpt->related);
    GList *rwcslist = g_hash_table_get_values(cpt->related);

    GList *rlist = rwrlist;
    GList *cslist = rwcslist;

    //Finishing index.html
    FILE *f = fopen("out/html/index.html", "a");
    if(f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    char buffer[2000];
    char *ptr;

    strcpy(buffer, cpt->name);
    for (ptr = buffer; *ptr != '\0'; ptr++)
        if (*ptr == ' ')
            *ptr = '-';

    fprintf(f, "\t\t<li><a href=\"%s.html\">%s</a></li></br>\n", buffer, cpt->name);
    printf("%s ->\n", cpt->name);
    ///////

    //Inicializing the concept file
    char filename[2000];
    sprintf(filename, "out/html/%s.html", buffer);

    FILE *cptf = fopen(filename, "w");
    if(cptf == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(cptf, "<!DOCTYPE html>\n<html>\n<head>\n\t<title>PL</title>\n");
    fprintf(cptf, "\t<meta name='viewport' content='width=device-width, initial-scale=1'>\n");
    fprintf(cptf, "\t<style>\n");
    fprintf(cptf, "\t\tbody { max-width: 1300px; margin: auto; }\n");
    fprintf(cptf, "\t</style>\n</head>\n");

    //Body
    fprintf(cptf, "<body>\n");
    fprintf(cptf, "\t<h1><p align='center'><font color='#2874A6'>%s</font></p></h1></br>\n", cpt->name);

    GList * rwtransk = g_hash_table_get_keys(cpt->translation);
    GList * rwtransv = g_hash_table_get_values(cpt->translation);
    GList * transk = rwtransk;
    GList * transv = rwtransv;

    if( transk && transv ){
        fprintf(cptf, "\t<h4><b>Translations:</b></h4>\n");
    }

    while( transk && transv ){
        fprintf(cptf, "\t\t<li>%s ==> %s</li></br>\n", (char *)transk->data, (char *)transv->data);
        printf("\t%s =====> %s \n", (char*)transk->data, (char *)transv->data);

        transv = transv->next;
        transk = transk->next;
    }

    g_list_free(rwtransk);
    g_list_free(rwtransv);

    while( rlist && cslist ){
        Relation r = (Relation)rlist->data;
        ConceptSet cs = (ConceptSet)cslist->data;
        GList * clist = g_hash_table_get_keys(cs);

        fclose(cptf);
        char * relationName = showRelation(r, filename);

        //Open the concept file again
        FILE *cptf = fopen(filename, "a");
        if(cptf == NULL) {
            printf("Error opening file %s!\n", filename);
            exit(1);
        }
        //////

        //Create/Open the relation grafo
        char grafoFilename[2000];
        sprintf(grafoFilename, "out/grafos/%sgrafo.dot", relationName);

        FILE *grafo = fopen(grafoFilename, "a");
        if(grafo == NULL) {
            printf("Error opening file %s!\n", grafoFilename);
            exit(1);
        }
        //////

        for( GList* cur = clist; cur; cur = cur->next ){
            Concept innercn = (Concept)cur->data;

            //Print relation attribute to grafo file
            fprintf(grafo, "\t\"%s\" -> \"%s\"\n", cpt->name, innercn->name);

            //Print relation attribute to concept file
            fprintf(cptf, "\t\t<li>%s</li></br>\n", innercn->name);
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
