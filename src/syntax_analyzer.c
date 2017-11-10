#include "syntax_analyzer.h"
#include "string.h"
#include "stdbool.h"
/**
*
*
*
*
*
*
**/

char ArtPreTB [15][15] = {
    // < - 1, > - 2, = - 3, ' ' - 0
    //     i + - * ( ) e / \ = n l g < >
/*id */   {0,2,2,2,0,2,2,2,2,2,2,2,2,2,2},
/* + */   {1,2,2,1,1,2,2,1,1,2,2,2,2,2,2},
/* - */   {1,2,2,1,1,2,2,1,1,2,2,2,2,2,2},
/* * */   {1,2,2,2,1,2,2,2,2,2,2,2,2,2,2},
/* ( */   {1,1,1,1,1,3,0,1,1,1,1,1,1,1,1},
/* ) */   {0,2,2,2,0,2,2,2,2,2,2,2,2,2,2},
/* e */   {1,1,1,1,1,3,3,1,1,1,1,1,1,1,1},
/* / */   {1,2,2,2,1,2,2,2,2,2,2,2,2,2,2},
/* \ */   {1,2,2,1,1,2,2,1,2,2,2,2,2,2,2},
/*eql*/   {1,1,1,1,1,2,2,1,1,2,2,2,2,2,2},
/*neq*/   {1,1,1,1,1,2,2,1,1,2,2,2,2,2,2},
/*leq*/   {1,1,1,1,1,2,2,1,1,2,2,2,2,2,2},
/*geq*/   {1,1,1,1,1,2,2,1,1,2,2,2,2,2,2},
/*les*/   {1,1,1,1,1,2,2,1,1,2,2,2,2,2,2},
/*gre*/   {1,1,1,1,1,2,2,1,1,2,2,2,2,2,2}

};




#define MAX_STACK 20
typedef struct {
    char arr[MAX_STACK];                             /* pole pro uložení hodnot */
    int top;                                /* index prvku na vrcholu zásobníku */
} tStack;

int STACK_SIZE = MAX_STACK;

void stackInit ( tStack* s ) {
    if (s==NULL){
        printf("stackerror");
    }
    else{
        s->top = -1;  //inicializace vrcholu
    }
}

int stackEmpty ( const tStack* s ) {
    return s->top==-1?1:0;
}

int stackFull ( const tStack* s ) {
    return s->top==STACK_SIZE-1?1:0;
}

void stackTop ( const tStack* s, char* c ) {
    if (stackEmpty(s)){ //abychom nesahali nahodne do pameti
        printf("stackerror");
    }
    else {
        *c=s->arr[s->top];
    }
}


void stackPop ( tStack* s ) {
    if (stackEmpty(s)){
        //tady by bylo to varovaní
    }
    else
    {
        s->top--;
    }
}


void stackPush ( tStack* s, char c ) {
    if (stackFull(s)){
        printf("stackerror");
    }
    else
    {
        s->top++;						//posunu a vlozim
        s->arr[s->top]=c;
    }
}


int ExpRes(){
    tStack *local;
    local = (tStack*)malloc(sizeof(tStack));
}

/*******
 *
 * Shora dolur
 *
 ****************/

int SyntaxAnalyzer(){
    Token *act,*next;
    int err;
    GetToken(&(act));
    //printf("%s",act->val);
    if (act->type==KEYWORD){
        printf("nasel jsem klicove slovo");
        err=Keyword(act);
        printf("serious ? ");
    }
    else { return false;}
    if (err==false){
        return false;
    }

}


int Keyword(Token *act){
    int err;
    if(strcmp(act->val,"dim")==0){
        Dim();
    }
    else if(strcmp(act->val,"as")==0){
        As();
    }
    else if(strcmp(act->val,"integer")==0){
        Int();
    }
    else if(strcmp(act->val,"double")==0){
        Dou();
    }
    else if(strcmp(act->val,"string")==0){
        Str();
    }
    else if (strcmp(act->val,"declare")==0){
        Declare();
    }
    else{
        return false;
    }
    if (err==false){
        return false;
    }

}

int Dim(){
    Token *act, *next;
    GetToken(&(act));
    if (act->type==ID){
        printf("trikrat slava musim volat ");
        GetToken(&(next));
        Keyword(next);
    }
}

int As(){
    Token *act,*next;
    GetToken(&(act));// toto je spatne potrevuju overit keyword a v jestli je v nem int nebo double nebo str
    if (act->type==KEYWORD){
        if (strcmp(act->val,"integer")==0){
            Keyword(act);
        }
        else if (strcmp(act->val,"double")==0){
            Keyword(act);
        }
        else if (strcmp(act->val,"string")==0){
            Keyword(act);
        }
        else{
            return false;
        }
    }
    else if(act->type==DOUBLE){

    }
    else if(act->type==STRING){

    }
    else{
        return  false;
    }
}

int Eql(){
    Token *act,*next;
    GetToken(&(act));
    if (act->type==INTEGER){
        PreAnalyzer(act);
    }
    else if (act->type==DOUBLE){
        PreAnalyzer(act);
    }
    else if (act->type==STRING){
        GetToken(&(next));
        if(next->type==EOL){
            return true;
        }
        else{
            return false;
        }
    }
    else if (act->type ==LBRACKET){
        PreAnalyzer(act);
    }
    else { return false;}
}
int Int(){
    printf("az na konec csveta a jeste dal!!! ");
    Token *act;
    GetToken(&(act));
    if (act->type==EQL){
        Eql();
    }
    else if(act->type==EOL){
    return true;
    }
}
int Dou(){
    return true;
}
int Str(){
    return true;
}
int Declare(){
    Function();
}
int Function(){
    Token *act;
    GetToken(&(act));
        if (act->type==KEYWORD&&(strcmp(act->val, "function")==0)){
            Keyword(act);
        }

}



//zdola nahoru

int PreAnalyzer(Token *act){
    printf("%s", act->val);
    return true;
}
