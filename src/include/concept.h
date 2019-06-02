#ifndef CONCEPT__H
#define CONCEPT__H

#import <glib.h>
#import "relation.h"

typedef struct concept *Concept;
typedef GHashTable * ConceptSet;

void linkConcepts(Concept source, Relation r, Concept associatee);

Concept mkConcept( const char * name );
void unmkConcept( Concept cpt );
ConceptSet newConceptSetfromHashTable();

void showConcept( Concept cpt );

void translation( Concept cpt, const char * lang, const char * translated );

#endif