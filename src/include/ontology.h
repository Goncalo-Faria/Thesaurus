#ifndef ONTOLOGY__H
#define ONTOLOGY__H

#include "relation.h"
#include "concept.h"

typedef struct ontology *Ontology;

Ontology mkOntology();
void showOntology( Ontology saurus);

void setBaseLanguage( Ontology saurus, const char * lang);
void addLanguage( Ontology saurus, const char * lang);
void addTitle(Ontology saurus, const char* title);

void unmkOntology( Ontology saurus );

Relation getRelation( Ontology saurus, const char* relationname );
Concept getConcept( Ontology saurus, const char* conceptname );

void relateRaw( Ontology saurus, const char* subclasse/*NT*/, const char* superclasse/*BT*/);
void relate( Ontology saurus, Relation subclasse/*NT*/, Relation superclasse/*BT*/);
void associate( Ontology saurus, Concept source, const char * relation, const char * assoc);

#endif