#include "dl_list.h"
#include "string.h"
#include "syntax_analyzer.h"



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
** Provede inicializaci seznamu L i s $
**/
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
	DLInsertFirst(L, '$', NULL);
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L + free
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

void DLInsertFirst (tDLList *L, char val, Token *act) {
/*
** Vloží nový prvek na začátek seznamu L.
**/
tDLElemPtr nov;
nov= (tDLElemPtr)malloc(sizeof(struct tDLElem));
if (nov==NULL)
{

}
	nov->data=val;
	nov->act=act;
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

void DLInsertLast(tDLList *L, char val, Token *act) {
/*
** Vloží nový prvek na konec seznamu L
**/
  tDLElemPtr nov;
  nov= (tDLElemPtr)malloc(sizeof(struct tDLElem)); //alokuji misto pro novy prvek
  if (nov==NULL){ //pokud nastane chyba alokace

  }
  	nov->data=val;
	nov->act=act;
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
**/
L->Act=L->First;


}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
**/
L->Act=L->Last;
}

void DLCopyFirst (tDLList *L, char *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
**/
if(L->First==NULL){

}
else{
	*val=L->First->data;
}
}

void DLCopyLast (tDLList *L, char *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
**/
if(L->Last==NULL){

}
else{
	*val=L->Last->data;
}
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L.
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
** Zruší poslední prvek seznamu L.
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
** Zruší prvek před aktivním prvkem seznamu L.
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

**/
	if(L->Act!=NULL){
		tDLElemPtr nov;
		nov= (tDLElemPtr)malloc(sizeof(struct tDLElem)); //alokuji misto pro novy prvek
		if (nov==NULL){ //pokud nastane chyba alokace

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
**/
if(L->Act!=NULL){
	tDLElemPtr nov;
	nov= (tDLElemPtr)malloc(sizeof(struct tDLElem)); //alokuji misto pro novy prvek
	if (nov==NULL){ //pokud nastane chyba alokace

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
**/
if(L->Act!=NULL){
	L->Act=L->Act->rptr;
}
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
**/
if(L->Act!=NULL){
	L->Act=L->Act->lptr;
}
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
**/
return (L->Act!=NULL)?1:0;
}

void DLCutUntil(tDLList *L, tDLList *V)
{
    DLDisposeList(V);
    DLLast(L);
    while((L->Act->data!= '<')&&(L->Act->data!= '$')){
        DLInsertFirst(V,L->Act->data, L->Act->act);
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
void DLLastTerm(tDLList* L){
	DLLast(L);
	while(PrePosition(L->Act->data)==-1){ //jde seznamem odzadu a pusuzuje zda nasla terminal
		DLPred(L);
	}
	// kdzy najdu tak je na nem nastavena aktivita
}
