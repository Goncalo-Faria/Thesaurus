#include "include/relation.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct relation{
    char * name;
} * Relation;

Relation mkRelation( const char * name ){

    Relation r = (Relation)malloc( sizeof(struct relation) );
    r->name = strdup(name);
    return r;
}

void unmkRelation( Relation r){
    free(r->name);
    free(r);
}

char * showRelation( Relation r, char * filename ){
    FILE *cptf = fopen(filename, "a");
    if(cptf == NULL) {
        printf("Error opening file %s!\n", filename);
        exit(1);
    }

    fprintf(cptf, "\t<h4><b>%s:</b></h4>\n", r->name);
    printf("\t%s \n ", r->name);

    fclose(cptf);

    return r->name;
}

char * getRelationName( Relation r ){
    return r->name;
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
        unmkRelation,
        NULL
    );
}
