#ifndef CONCEPT__H
#define CONCEPT__H

#import <glib.h>

typedef struct concept *Concept;
typedef GHashTable * ConceptSet;

void link(Concept source, Relation r, Concept associatee);

Concept mkConcept( const char * name );
void unmkConcept( Concept cpt );
Concept clone( Concept cpt );
ConceptSet newConceptSetfromHashTable();

#endif