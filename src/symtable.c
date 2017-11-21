#include "symtable.h"


/******* základní operace nad tabulkou ********/

int DJBHash(char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;
  return hash % MAX_TSIZE;
}


void TInit(Table* t) {
  for (int i = 0; i < MAX_TSIZE; i++) {
    (*t)[i] = NULL;
  }
}


bool TSearch (Table* t, char* name, bool *isdef, bool define) {
  int hash = DJBHash(name);
  tItem* ptr = (*t)[hash];
  while (ptr != NULL) {
    if (!strcmp(ptr->name, name)){
        if(isdef != NULL)
            *isdef = ptr->isdef;
        if(define)
            ptr->isdef = true;
        return true;
    }
    else
      ptr = ptr->next;
  }
  return false;
}


tItem* TRead (Table* t, char* name) {
  int hash = DJBHash(name);
  tItem* ptr = (*t)[hash];
  while (ptr != NULL) {
    if (!strcmp(ptr->name, name))
      return ptr;
    else
      ptr = ptr->next;
  }
  return NULL;
}


void TInsert (Table* t, tItem *item) {
  tItem* oldItem;
  if ((oldItem = TRead(t, item->name)) != NULL)
    oldItem = item;
  else {
    int hash = DJBHash(item->name);
    item->next = (*t)[hash];
    (*t)[hash] = item;
  }
}


void TDelete (Table* t, char* name) {
  int hash = DJBHash(name);
  tItem* ptr = (*t)[hash];
  tItem* prevptr = NULL;
  while (ptr != NULL) {
    if (!strcmp(ptr->name, name)) {
      if (prevptr == NULL)
        (*t)[hash] = ptr->next;
      else
        prevptr = ptr->next;
      free(ptr);
      ptr = NULL;
    }
    else {
      prevptr = ptr;
      ptr = ptr->next;
    }
  }
}


void TClearAll (Table* t) {
  tItem *ptr;
  tItem *nextptr;
  for (int i = 0; i < MAX_TSIZE; i++) {
    ptr = (*t)[i];
    while (ptr != NULL) {
      nextptr = ptr->next;
      free(ptr->name);
      free(ptr);
      ptr = nextptr;
    }
  }
  TInit(t);
}


/******* pokročilé operace nad tabulkou *******/

bool Symtable_Init() {
  Func = (Table *) malloc(sizeof(Table));
  if (Func == NULL)
    return false;

  TInit(Func);

  return true;
}

void Symtable_Destroy() {
  tItem *ptr;
  tItem *nextptr;
  for (int i = 0; i < MAX_TSIZE; i++) {
    ptr = (*Func)[i];
    while (ptr != NULL) {
      nextptr = ptr->next;
      TClearAll(ptr->arg);
      TClearAll(ptr->var);
      ptr = nextptr;
    }
  }
  TClearAll(Func);
  free(Func);
}


bool Dec_Func(char *funcname, bool define) {
  tItem *item = (tItem *) malloc(sizeof(tItem));
  if (item == NULL)
    return false;

  item->name = funcname;
  item->isdef = define;
  item->type = EOL;
  item->arg = (Table *) malloc(sizeof(Table));
  item->var = (Table *) malloc(sizeof(Table));
  if (item->arg == NULL || item->var == NULL)
    return false;

  TInit(item->arg);
  TInit(item->var);
  TInsert(Func, item);

  return true;
}

bool Dec_Func_Set_Type(char *funcname, TokType type) {
  tItem *item = TRead(Func, funcname);
  if (item == NULL)
    return false;

  item->type = type;

  return true;
}

bool Dec_Func_AddArgument(char *funcname, int n, TokType argtype) {
  tItem *func = TRead(Func, funcname);
  if (func == NULL)
    return false;

  tItem *newArg = (tItem *) malloc(sizeof(tItem));
  if (newArg == NULL)
    return false;

  char *name = (char *) malloc(sizeof(char)*12);
  sprintf(name, "%d", n);
  newArg->name = name;
  newArg->type = argtype;

  TInsert(func->arg, newArg);

  return true;
}

bool Add_Var(char *funcname, char *varname, TokType vartype) {
  tItem *func = TRead(Func, funcname);
  if (func == NULL)
    return false;

  tItem *newVar = (tItem *) malloc(sizeof(tItem));
  if (newVar == NULL)
    return false;

  newVar->name = varname;
  newVar->type = vartype;

  TInsert(func->var, newVar);

  return true;
}

bool Define_Func(char *funcname) {
  return TSearch(Func, funcname, NULL, true);
}

bool Search_Func(char *funcname, bool *isdef) {
  return TSearch(Func, funcname, isdef, false);
}

bool Ret_Func_Type(char *funcname, TokType rettype) {
  tItem *item = TRead(Func, funcname);
  if (item == NULL)
    return false;

  if (item->type == rettype)
    return true;
  else
    return false;
}

bool Nth_Func_ArgType(char *funcname, int n, TokType argtype) {
  tItem *item = TRead(Func, funcname);
  if (item == NULL)
    return false;

  char *name = (char *) malloc(sizeof(char)*12);
  sprintf(name, "%d", n);
  item = TRead(item->arg, name);
  if (item == NULL)
    return false;

  if (item->type == argtype)
    return true;
  else
    return false;
}

bool Search_Var(char *funcname, char *varname, TokType *vartype) {
  tItem *item = TRead(Func, funcname);
  if (item == NULL)
    return false;

  item = TRead(item->var, varname);
  if (item == NULL)
    return false;

  if (vartype != NULL)
    *vartype = item->type;

    return true;
}
