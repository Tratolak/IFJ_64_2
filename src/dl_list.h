#include<stdio.h>
#include<stdlib.h>
#include "scanner.h"

//Martin Stodůlka(xstodu08)
//Ondřej Olšák(xolsak00)
//Michael Schneider(xschne07)
//Marek Kuchynka(xkuchy00)

typedef struct tDLElem {                 // prvek dvousměrně vázaného seznamu
        char data;   // terminal popr. neterminal
        Token *act; //ukayatal na token k prislusnemu terminalu
        struct tDLElem *lptr;          // ukazatel na předchozí prvek seznamu
        struct tDLElem *rptr;        // ukazatel na následující prvek seznamu
} *tDLElemPtr;

typedef struct {                                  // dvousměrně vázaný seznam
    tDLElemPtr First;                      //ukazatel na první prvek seznamu
    tDLElemPtr Act;                    //  ukazatel na aktuální prvek seznamu
    tDLElemPtr Last;                   // ukazatel na posledni prvek seznamu
} tDLList;


void DLInitList (tDLList *);
void DLInitList$ (tDLList *);
void DLDisposeList (tDLList *);
void DLInsertFirst (tDLList *, char ,Token*);
void DLInsertLast(tDLList *, char, Token*);
void DLFirst (tDLList *);
void DLLast (tDLList *); //aktivita na posledni prvek
void DLCopyFirst (tDLList *, char *);
void DLCopyLast (tDLList *, char *);
void DLDeleteFirst (tDLList *);
void DLDeleteLast (tDLList *);
void DLPostDelete (tDLList *);
void DLPreDelete (tDLList *);
void DLPostInsert (tDLList *, char);
void DLPreInsert (tDLList *, char);
void DLCopy (tDLList *, char *);
void DLActualize (tDLList *, char);
void DLSucc (tDLList *);
void DLPred (tDLList *);
int DLActive (tDLList *);
void DLCutUntil(tDLList *L, tDLList *V); //vykopirovani prvku ay po < nebo $
int DLCompare(tDLList *L, tDLList *V);//vraci 0 kdyz jsou stejne jinak 1
void DLCopyLastTerm (tDLList *L, char *val);//do char vrati nejlevejsi terminal
void DLLastTerm(tDLList *);

