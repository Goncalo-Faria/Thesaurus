#ifndef THESAURUS__H
#define THESAURUS__H

#include "relation.h"
#include "concept.h"

typedef struct saurus *Thesaurus;

Thesaurus mkThesaurus();

void putRelation( Thesaurus saurus , Relation rel);
Relation getRelation( Thesaurus saurus, const char* r);

void putConcept( Thesaurus, Concept cpt);
Concept getConcept( Thesaurus saurus, const char* r );

void relate( Thesaurus saurus, Relation subclasse/*NT*/, Relation superclasse/*BT*/);
void associate(Thesaurus saurus, Concept source, Relation r, Concept associatee);

#endif