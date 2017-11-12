
/* Předmět: Algoritmy (IAL) - FIT VUT v Brně
 * Hlavičkový soubor pro c206.c (Dvousměrně vázaný lineární seznam)
 * Vytvořil: Martin Tuček, září 2005
 * Upravil: Kamil Jeřábek, říjen 2017
 *  
 * Tento soubor, prosíme, neupravujte!  
 */

#include<stdio.h>
#include<stdlib.h>

#define FALSE 0
#define TRUE 1

extern int errflg;
extern int solved;
 
typedef struct tDLElem {                 /* prvek dvousměrně vázaného seznamu */ 
        char data;                                            /* užitečná data */
        struct tDLElem *lptr;          /* ukazatel na předchozí prvek seznamu */
        struct tDLElem *rptr;        /* ukazatel na následující prvek seznamu */
} *tDLElemPtr;

typedef struct {                                  /* dvousměrně vázaný seznam */
    tDLElemPtr First;                      /* ukazatel na první prvek seznamu */
    tDLElemPtr Act;                     /* ukazatel na aktuální prvek seznamu */
    tDLElemPtr Last;                    /* ukazatel na posledni prvek seznamu */
} tDLList;

                                             /* prototypy jednotlivých funkcí */
void DLInitList (tDLList *);
void DLInitList$ (tDLList *);
void DLDisposeList (tDLList *);
void DLInsertFirst (tDLList *, char);
void DLInsertLast(tDLList *, char);
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
int DLActive (tDLList *); //
void DLCutUntil(tDLList *L, tDLList *V); //vykopirovani prvku ay po < nebo $
int DLCompare(tDLList *L, tDLList *V);//vraci 0 kdyz jsou stejne jinak 1
void DLCopyLastTerm (tDLList *L, char *val);
/* Konec hlavičkového souboru c206.h */
