#ifndef RELATION____H
#define RELATION____H

#import <glib.h>

typedef struct relation *Relation;

typedef GHashTable * RelationSet;

Relation mkRelation( const char * name );
void unmkRelation( Relation r);
Relation clone( Relation rel );

RelationSet newRelationSetfromHashTable();

unsigned int hashRelation( Relation key );
int equalRelation( Relation a, Relation b);

#endif