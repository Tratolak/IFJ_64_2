#include "dl_list.h"
#include "syntax_analyzer.h"

//Martin Stodůlka(xstodu08)
//Ondřej Olšák(xolsak00)
//Michael Schneider(xschne07)
//Marek Kuchynka(xkuchy00)

/**
 * Init nastaveni ukayatelu na NULL
 * @param L dllist
 */
void DLInitList (tDLList *L) {
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

/**
 * init pro precedncni analyzu, prida $ na zacatek
 * @param L dllist
 */
void DLInitList$ (tDLList *L) {

	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
	DLInsertFirst(L, '$', NULL);
}

/**
 * rozlozi list i uvolni
 * @param L dllist
 */
void DLDisposeList (tDLList *L) {
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

/**
 * vlozi prvek na zacatek
 * @param L dllist
 * @param val char v prvku
 * @param act ukayatel na reprezentujici token
 */
void DLInsertFirst (tDLList *L, char val, Token *act) {
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

/**
 * vlozi prvek na konec
 * @param L dllist
 * @param val char v prvku
 * @param act ukazatel na reprezentujici token
 */
void DLInsertLast(tDLList *L, char val, Token *act) {

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

/**
 * nastavi act na prvni prvek
 * @param L dllist
 */
void DLFirst (tDLList *L) {
L->Act=L->First;


}

/**
 * nastavi act na posledni prvek
 * @param L dllist
 */
void DLLast (tDLList *L) {

L->Act=L->Last;
}

/**
 * vrati char prvniho prvku (pouziti prevazne pro kontrolu)
 * @param L dllist
 * @param val ukazatel na jehoz adressu se vlozi char
 */
void DLCopyFirst (tDLList *L, char *val) {

if(L->First==NULL){

}
else{
	*val=L->First->data;
}
}

/**
 * vrati char posledniho prvku (pouziti prevazne pro kontrolu)
 * @param L dllist
 * @param val ukazatel na jehoz adressu se vlozi char
 */
void DLCopyLast (tDLList *L, char *val) {

if(L->Last==NULL){

}
else{
	*val=L->Last->data;
}
}

/**
 * smaze prvni prvek seznamu i s free
 * @param L dllist
 */
void DLDeleteFirst (tDLList *L) {

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

/**
 * smaze posledni prvek seznamu
 * @param L dllist
 */
void DLDeleteLast (tDLList *L) {
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

/**
 * smaze nasledujici prvek za act
 * @param L dllist
 */
void DLPostDelete (tDLList *L) {

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
/**
 * smaze predchozi prvek pred act
 * @param L dllist
 */
void DLPreDelete (tDLList *L) {

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

/**
 * funkce pro vlozeni prvku do dllistu za act (vyuziti na debug)
 * @param L dllist
 * @param val char vlozeni
 */
void DLPostInsert (tDLList *L, char val) {

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

/**
 * funkce pro vlozeni prvku do dllistu pred act (vyuziti na debug)
 * @param L dllist
 * @param val char vlozeni
 */
void DLPreInsert (tDLList *L, char val) {

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

/**
 * vraci hodnotu charu ve val na act prvku
 * @param L dllist
 * @param val ukazatel na char
 */
void DLCopy (tDLList *L, char *val) {

if(L->Act!=NULL){
	*val=L->Act->data;
}
else{

}
}

/**
 * prepise obsah char na act prvku
 * @param L dllist
 * @param val hodnota kterou se prepisuje
 */
void DLActualize (tDLList *L, char val) {

if(L->Act!=NULL){
	L->Act->data=val;
}

}

/**
 * act na nasledujici prvek
 * @param L dllist
 */
void DLSucc (tDLList *L) {

if(L->Act!=NULL){
	L->Act=L->Act->rptr;
}
}

/**
 * act na predchozi prvek
 * @param L dllist
 */
void DLPred (tDLList *L) {

if(L->Act!=NULL){
	L->Act=L->Act->lptr;
}
}

/**
 * vraci jestli je aktivni
 * @param L dllist
 * @return 1-je aktivni, 0-neni aktivni
 */
int DLActive (tDLList *L) {

return (L->Act!=NULL)?1:0;
}

/**
 * funkce pro vyjmuti prvku po zarazku z local (prec analyza)
 * @param L dllist local
 * @param V ldlist vysledny dllist
 */
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

/**
 * porovnani dllistu
 * @param L prvni dllist
 * @param V druhy dllist
 * @return 0-kdyz jsou stejne, 1- jsou rozdilne
 */
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

/**
 * vrati posledni terminal
 * @param L dllist
 * @param val ukaztel v kterem se vrati posledni terminal
 */
void DLCopyLastTerm (tDLList *L, char *val) {

   DLLast(L);
    while(PrePosition(L->Act->data)==-1){ //jde seznamem odzadu a pusuzuje zda nasla terminal
        DLPred(L);
    }
    *val=L->Act->data; //terminal vrati

}
/**
 * nastavi act na terminal
 * @param L dllist
 */
void DLLastTerm(tDLList* L){
	DLLast(L);
	while(PrePosition(L->Act->data)==-1){ //jde seznamem odzadu a pusuzuje zda nasla terminal
		DLPred(L);
	}
	// kdzy najdu tak je na nem nastavena aktivita
}
