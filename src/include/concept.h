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

unsigned int hashConcept( Concept key );
int equalConcept( Concept a, Concept b);

void showConcept( Concept cpt );

int isRelatedBy( Concept cpt, Relation edge);

GList * identifyRelations(Concept origin, Concept dest);

void fillConceptHistogram(Concept cpt, Relation edge, GHashTable * conceptHist);

void translation( Concept cpt, const char * lang, const char * translated );

#endif