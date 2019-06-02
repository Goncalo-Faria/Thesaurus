#ifndef THESAURUS__H
#define THESAURUS__H

#include "relation.h"
#include "concept.h"

typedef struct thesaurus *Thesaurus;

Thesaurus mkThesaurus();
void showThesaurus(Thesaurus saurus);

Relation getRelation( Thesaurus saurus, const char* relationname );
Concept getConcept( Thesaurus saurus, const char* conceptname );

void relateRaw(Thesaurus saurus, const char* subclasse/*NT*/, const char* superclasse/*BT*/);
void relate( Thesaurus saurus, Relation subclasse/*NT*/, Relation superclasse/*BT*/);
void associate(Thesaurus saurus, Concept source, Relation r, Concept associatee);

#endif