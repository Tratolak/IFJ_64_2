#include "symtable.h"


int DJBHash(char *str) {
  unsigned long hash = 5381;
  int c;
  while (c = *str++)
    hash = ((hash << 5) + hash) + c;
  return hash % MAX_TSIZE;
}


void TInit(Table* t) {
  for (int i = 0; i < MAX_TSIZE; i++) {
    (*t)[i] = NULL;
  }
}


bool TSearch (Table* t, char* name) {
  int hash = DJBHash(name);
  tItem* ptr = (*t)[hash];
  while (ptr != NULL) {
    if (!strcmp(ptr->name, name))
      return true;
    else
      ptr = ptr->next;
  }
  return false;
}


tData* TRead (Table* t, char* name) {
  int hash = DJBHash(name);
  tItem* ptr = (*t)[hash];
  while (ptr != NULL) {
    if (!strcmp(ptr->name, name))
      return &(ptr->data);
    else
      ptr = ptr->next;
  }
  return NULL;
}


bool TInsert (Table* t, char* name, tData data) {
  tData* oldData;
  if ((oldData = TRead(t, name)) != NULL) {
    *oldData = data;
    return true;
  }
  else {
    int hash = DJBHash(name);
    tItem* ptr = (tItem *) malloc(sizeof(tItem));
    if (ptr == NULL)
      return false;
    else {
      ptr->name = name;
      ptr->data = data;
      ptr->next = (*t)[hash];
      (*t)[hash] = ptr;
      return true;
    }
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
      free(ptr);
      ptr = nextptr;
    }
  }
  TInit(t);
}
