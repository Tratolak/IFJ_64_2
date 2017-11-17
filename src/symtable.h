#ifndef SYMTABLE_H_INCLUDED
#define SYMTABLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_TSIZE 101


typedef struct tData{
	int val;//example, TBD
} tData;

typedef struct tItem{
  char* name;
	tData data;
	struct tItem* next;
} tItem;

typedef tItem* Table[MAX_TSIZE];


void TInit(Table* t);

bool TSearch (Table* t, char* name);

tData* TRead (Table* t, char* name);

bool TInsert (Table* t, char* name, tData data);

void TDelete (Table* t, char* name);

void TClearAll (Table* t);

#endif // SYMTABLE_H_INCLUDED
