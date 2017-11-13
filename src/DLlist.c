#include "DLlist.h"
#include "string.h"
#include "syntax_analyzer.h"

int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}
void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím
**/
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLInitList$ (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
	DLInsertFirst(L, '$');
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free.
**/
if(L->First != NULL){
while(L->First != L->Last)
{
	L->Act = L->Last->lptr;
	free(L->Last);
	L->Last=L->Act;
}
free(L->First); //posledni prvek seznamu
L->First=NULL;
L->Act = NULL;
L->Last = NULL;
}
else
{
	L->Act = NULL;
	L->Last = NULL;
}

}

void DLInsertFirst (tDLList *L, char val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
tDLElemPtr nov;
nov= (tDLElemPtr)malloc(sizeof(struct tDLElem));
if (nov==NULL)
{
	DLError();
}
nov->data=val;
nov->lptr=NULL;
nov->rptr=L->First;

	if(L->First==NULL)
	{
		L->Last=nov;
	}
	else
	{
		L->First->lptr=nov;
	}
L->First=nov;
}

void DLInsertLast(tDLList *L, char val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  tDLElemPtr nov;
  nov= (tDLElemPtr)malloc(sizeof(struct tDLElem)); //alokuji misto pro novy prvek
  if (nov==NULL){ //pokud nastane chyba alokace
  	DLError();
  }
  nov->data=val;
	nov->lptr=L->Last;
	nov->rptr=NULL;

  if(L->Last==NULL){//pokud pridavam na prazdny seznam
		L->First=nov;
	}
	else{
	  L->Last->rptr=nov;
	}
	L->Last=nov;
}


void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
L->Act=L->First;


}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
L->Act=L->Last;
}

void DLCopyFirst (tDLList *L, char *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
if(L->First==NULL){
	 DLError();
}
else{
	*val=L->First->data;
}
}

void DLCopyLast (tDLList *L, char *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
if(L->Last==NULL){
	 DLError();
}
else{
	*val=L->Last->data;
}
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	if(L->First!=NULL){
		if (L->First==L->Act){ //zruseni act kdyz ukazuje na prvni
			L->Act=NULL;
		}
		if(L->First->rptr!=NULL){ //jdu pres dva prvky
			L->First=L->First->rptr;
			free(L->First->lptr);
			L->First->lptr=NULL;
		}
		else{
			free(L->First);
			L->First=NULL;
			L->Last=NULL;
		}

	}
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	if(L->Last!=NULL){ //zda seznam neni prazdny
		if (L->Last==L->Act){ //ruseni aktualniho prvku
			L->Act=NULL;
		}
		if(L->Last->lptr!=NULL){ //overeni zda existuje prvek pred poslednim
			L->Last=L->Last->lptr;
			free(L->Last->rptr);
			L->Last->rptr=NULL;
		}
		else{ //seznam velikosti jeden prvek
			free(L->Last);
			L->Last=NULL;
			L->First=NULL;
		}

	}

}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
tDLElemPtr pom;
if(L->Act!=NULL && (L->Act!=L->Last)){
	pom= L->Act->rptr->rptr;
	if(pom!=NULL){
		free(L->Act->rptr);
		pom->lptr=L->Act;
		L->Act->rptr=pom;
	}
	else
	{
		free(L->Act->rptr);
		L->Last=L->Act;
		L->Act->rptr=NULL;
	}

}


}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
  tDLElemPtr pom;
	if(L->Act!=NULL && (L->Act!=L->First)){
		pom= L->Act->lptr->lptr;
		if(pom!=NULL){
			free(L->Act->lptr);
			pom->rptr=L->Act;
			L->Act->lptr=pom;
		}
		else
		{
			free(L->Act->lptr);
			L->First=L->Act;
			L->Act->lptr=NULL;
		}

	}


}

void DLPostInsert (tDLList *L, char val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act!=NULL){
		tDLElemPtr nov;
		nov= (tDLElemPtr)malloc(sizeof(struct tDLElem)); //alokuji misto pro novy prvek
		if (nov==NULL){ //pokud nastane chyba alokace
			DLError();
		}
		nov->data=val;
		if (L->Act->rptr!=NULL){
		L->Act->rptr->lptr=nov;
		nov->lptr=L->Act;
		nov->rptr=L->Act->rptr;
		}
		else{
			nov->lptr=L->Act;
			nov->rptr=NULL;
      L->Last=nov; //aktualizace posledniho prvku seznamu
		}
		L->Act->rptr=nov;
	}

}

void DLPreInsert (tDLList *L, char val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
if(L->Act!=NULL){
	tDLElemPtr nov;
	nov= (tDLElemPtr)malloc(sizeof(struct tDLElem)); //alokuji misto pro novy prvek
	if (nov==NULL){ //pokud nastane chyba alokace
		DLError();
	}
	nov->data=val;
	if (L->Act->lptr!=NULL){
	L->Act->lptr->rptr=nov;
	nov->rptr=L->Act;
	nov->lptr=L->Act->lptr;
	}
	else{
		nov->rptr=L->Act;
		nov->lptr=NULL;
    L->First=nov; //aktualizace prvniho prvku seznamu
	}
	L->Act->lptr=nov;
}
}
void DLCopy (tDLList *L, char *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

if(L->Act!=NULL){
	*val=L->Act->data;
}
else{
	DLError();
}
}

void DLActualize (tDLList *L, char val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
if(L->Act!=NULL){
	L->Act->data=val;
}

}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
if(L->Act!=NULL){
	L->Act=L->Act->rptr;
}
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
if(L->Act!=NULL){
	L->Act=L->Act->lptr;
}
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
return (L->Act!=NULL)?1:0;
}

void DLCutUntil(tDLList *L, tDLList *V)
{
    DLDisposeList(V);
    DLLast(L);
    while((L->Act->data!= '<')&&(L->Act->data!= '$')){
        DLInsertFirst(V,L->Act->data);
		DLPred(L);
		DLDeleteLast(L);
    }
	DLDeleteLast(L); // toto je to posledni ynacka <
}
int DLCompare(tDLList *L, tDLList *V){
    DLFirst(L);
    DLFirst(V);
    while((DLActive(L)!=0)&&(DLActive(V)!=0)&&(L->Act->data==V->Act->data)){
        DLSucc(L);
        DLSucc(V);
    }
    if ((DLActive(L)==0)&&(DLActive(V)==0)){
        return 0;
    }
    else{
        return 1;
    }


}

void DLCopyLastTerm (tDLList *L, char *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního terminalu seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
   DLLast(L);
    while(PrePosition(L->Act->data)==-1){ //jde seznamem odzadu a pusuzuje zda nasla terminal
        DLPred(L);
    }
    *val=L->Act->data; //terminal vrati

}