#include "symtable.h"

//Martin Stodůlka(xstodu08)
//Ondřej Olšák(xolsak00)
//Michael Schneider(xschne07)
//Marek Kuchynka(xkuchy00)

/******* základní operace nad tabulkou ********/

/**
 * Zahashuje řetězec (funkce převzata z IAL přednášek)
 * @param  str řetězec pro zahashování
 * @return     klíč (index) do tabulky
 */
int DJBHash(char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;
  return hash % MAX_TSIZE;
}

/**
 * Inicializuje prázdnou tabulku do základního stavu
 * @param t ukazatel na tabulku
 */
void TInit(Table* t) {
  for (int i = 0; i < MAX_TSIZE; i++) {
    (*t)[i] = NULL;
  }
}

/**
 * Vyhledá v tabulce položku a zjistí její parametry
 * @param  t         ukazatel na tabulku
 * @param  name      jméno hledané funkce/proměnné/argumentu
 * @param  isdef     je definována?
 * @param  define    definujeme funkci
 * @param  numofargs počet argumentů funkce
 * @return           true  pokud byla nalezena funkce s tímto jménem
 *                   false pokud nebyla nalezena funkce s tímto jménem
 */
bool TSearch (Table* t, char* name, bool *isdef, bool define, int *numofargs) {
  int hash = DJBHash(name);
  tItem* ptr = (*t)[hash];
  while (ptr != NULL) {
    if (!strcmp(ptr->name, name)){
        if(isdef != NULL)
            *isdef = ptr->isdef;
        if(numofargs != NULL)
            *numofargs = ptr->numofargs;
        if(define)
            ptr->isdef = true;
        return true;
    }
    else
      ptr = ptr->next;
  }
  return false;
}

/**
 * Přečte prvek z tabulky
 * @param  t    ukazatel na tabulku
 * @param  name jméno hledané funkce/proměnné/argumentu
 * @return      ukazatel na nalezený prvek v tabulce
 *              NULL pokud prvek nenajde
 */
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

/**
 * Vloží prvek do tabulky, příp. přepíše data prvku, u kterého se shoduje jméno
 * @param t    ukazatel na tabulku
 * @param item ukazatel na prvek, který se má vložit
 */
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

/**
 * Vymaže prvek zadaného jména
 * @param t    ukazatel na tabulku
 * @param name jméno funkce/proměnné/argumentu
 */
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

/**
 * Vymaže všechny prvky tabulky tabulku
 * @param t ukazatel na tabulku
 */
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
  free(t);
}


/******* pokročilé operace nad tabulkou *******/

/**
 * Inicializuje globální tabulku symbolů
 * @return true  v případě úspěchu
 *         false v případě neúspěchu
 */
bool Symtable_Init() {
  Func = (Table *) malloc(sizeof(Table));
  if (Func == NULL)
    return false;

  TInit(Func);

  return true;
}

/**
 * Zruší celou tabulku symbolů
 */
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
}

/**
 * Ulozi prvek (funkci) do tabulky symbolu
 * @param funcname - jmeno funkce
 * @param define - zda-li se zaroven jedna o definici
 */
bool Dec_Func(char *funcname, bool define) {
  tItem *item = (tItem *) malloc(sizeof(tItem));
  if (item == NULL)
    return false;

  item->name = (char *) malloc(sizeof(char)* (strlen(funcname)+1));
  if(item->name == NULL)
    return false;
  strcpy(item->name, funcname);

  item->numofargs = 0;
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

/**
 * Nastavi navratovou hodnotu funkce
 * @param funcname - jmeno funkce
 * @param type - navratovy typ
 */
bool Dec_Func_Set_Type(char *funcname, TokType type) {
  tItem *item = TRead(Func, funcname);
  if (item == NULL)
    return false;

  item->type = type;

  return true;
}

/**
 * Nastavi nty argument funkce
 * @param funcname - jmeno funkce
 * @param n - cislo argumentu
 * @param argtype - typ argumentu
 */
bool Dec_Func_AddArgument(char *funcname, int n, TokType argtype) {
  tItem *func = TRead(Func, funcname);
  if (func == NULL)
    return false;

  func->numofargs++;
  tItem *newArg = (tItem *) malloc(sizeof(tItem));
  if (newArg == NULL)
    return false;

  char *name = (char *) malloc(sizeof(char) * 12);
  sprintf(name, "%d", n);
  newArg->name = name;
  newArg->type = argtype;

  TInsert(func->arg, newArg);

  return true;
}

/**
 * Deklaruje promennou uvnitr funkce
 * @param funcname - jmeno funkce
 * @param varname - nazev promenne
 * @param vartype - typ promenne
 */
bool Add_Var(char *funcname, char *varname, TokType vartype) {
  tItem *func = TRead(Func, funcname);
  if (func == NULL)
    return false;

  tItem *newVar = (tItem *) malloc(sizeof(tItem));
  if (newVar == NULL)
    return false;

  newVar->name = (char *) malloc(sizeof(char)* (strlen(varname)+1));
  if(newVar->name == NULL)
    return false;
  strcpy(newVar->name, varname);

  newVar->type = vartype;

  TInsert(func->var, newVar);

  return true;
}

/**
 * Definuje funkci
 * @param funcname - jmeno funkce
 */
bool Define_Func(char *funcname) {
  return TSearch(Func, funcname, NULL, true, NULL);
}

/**
 * Zkontroluje zda funkce je obsazena uvnitr tabulky, poprida vraci pocet argumentu a zda-li je definovana
 * @param funcname - jmeno funkce
 * @param isdef - je definovana
 * @param numofargs - pocet argumentu
 */
bool Search_Func(char *funcname, bool *isdef, int *numofargs) {
  return TSearch(Func, funcname, isdef, false, numofargs);
}

/**
 * Vraci navratovy typ funkce
 * @param funcname - jmeno funkce
 * @param rettype - navratovy typ
 */
bool Ret_Func_Type(char *funcname, TokType *rettype) {
  tItem *item = TRead(Func, funcname);
  if (item == NULL)
    return false;

  *rettype = item->type;
  return true;
}

/**
 * Zkontroluje zda-li nty argument funkce je urciteho typu
 * @param funcname - jmeno funkce
 * @param n - poradi argumentu
 * @param argtype - typ ktery je ocekavan
 */
bool Nth_Func_ArgType(char *funcname, int n, TokType argtype) {
  tItem *item = TRead(Func, funcname);
  if (item == NULL)
    return false;

  char name[12];
  sprintf(name, "%d", n);
  item = TRead(item->arg, name);
  if (item == NULL)
    return false;

  if (item->type == argtype)
    return true;
  else
    return false;
}

/**
 * Zkontroluje zda-li funkce obsahuje promennou/vrati jeji typ
 * @param funcname - jmeno funkce
 * @param varname - jmeno promenne
 * @param vartype - typ vyhledane promenne
 */
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

/**
 * Zkontroluje zda-li je kazda funkce definovana
 */
bool Every_Func_Defed(){
  tItem *ptr;
  tItem *nextptr;

  for(int i = 0; i < MAX_TSIZE; ++i){
    ptr = (*Func)[i];
    while (ptr != NULL) {
      nextptr = ptr->next;

      if(!ptr->isdef){
        return false;
      }

      ptr = nextptr;
    }
  }

  return true;
}
