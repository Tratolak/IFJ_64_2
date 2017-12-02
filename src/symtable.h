#ifndef SYMTABLE_H_INCLUDED
#define SYMTABLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"

#define MAX_TSIZE 101

//Martin Stodůlka(xstodu08)
//Ond�ej Olšák(xolsak00)
//Michael Schneider(xschne07)
//Marek Kuchynka(xkuchy00)

typedef struct tItem tItem;
typedef tItem* Table[MAX_TSIZE];

struct tItem{
  char* name;
  TokType type;
  bool isdef;
  int numofargs;
  Table *arg;
  Table *var;
	struct tItem* next;
};

Table *Func;


bool Symtable_Init();

void Symtable_Destroy();


bool Dec_Func(char *funcname, bool define);

bool Dec_Func_Set_Type(char *funcname, TokType type);

bool Dec_Func_AddArgument(char *funcname, int n, TokType argtype);

bool Add_Var(char *funcname, char *varname, TokType vartype);

bool Define_Func(char *funcname);


bool Search_Func(char *funcname, bool *isdef, int *numofargs);

bool Ret_Func_Type(char *funcname, TokType *rettype);

bool Nth_Func_ArgType(char *funcname, int n, TokType argtype);

bool Search_Var(char *funcname, char *varname, TokType *vartype);

bool Every_Func_Defed();


#endif // SYMTABLE_H_INCLUDED
