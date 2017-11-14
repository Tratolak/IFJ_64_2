#include "syntax_analyzer.h"
#include "symtable.h"
#include "code_generator.h"
#include "string.h"
#include "stdbool.h"
#include "DLlist.h"
#include "stack_operations.h"
/**
*
*
*
*
*
*
**/

int S_functype(bool declaration);
int S_funcArgList(Token *act, bool declaration);

char ArtPreTB [15][15] = {
    // < - 1, > - 2, = - 3, ' ' - 0
    //     i + - * ( ) $ / \ = n l g < >
/*id */   {0,2,2,2,0,2,2,2,2,2,2,2,2,2,2},
/* + */   {1,2,2,1,1,2,2,1,1,2,2,2,2,2,2},
/* - */   {1,2,2,1,1,2,2,1,1,2,2,2,2,2,2},
/* * */   {1,2,2,2,1,2,2,2,2,2,2,2,2,2,2},
/* ( */   {1,1,1,1,1,3,0,1,1,1,1,1,1,1,1},
/* ) */   {0,2,2,2,0,2,2,2,2,2,2,2,2,2,2},
/* $ */   {1,1,1,1,1,3,3,1,1,1,1,1,1,1,1},
/* / */   {1,2,2,2,1,2,2,2,2,2,2,2,2,2,2},
/* \ */   {1,2,2,1,1,2,2,1,2,2,2,2,2,2,2},
/*eql*/   {1,1,1,1,1,2,2,1,1,2,2,2,2,2,2},
/*neq*/   {1,1,1,1,1,2,2,1,1,2,2,2,2,2,2},
/*leq*/   {1,1,1,1,1,2,2,1,1,2,2,2,2,2,2},
/*geq*/   {1,1,1,1,1,2,2,1,1,2,2,2,2,2,2},
/*les*/   {1,1,1,1,1,2,2,1,1,2,2,2,2,2,2},
/*gre*/   {1,1,1,1,1,2,2,1,1,2,2,2,2,2,2}

};

Table sym_table;

int GTOKEN_RES;
bool IN_FUCTION = false;

#define GET_TOKEN(A)\
GTOKEN_RES = GetToken(&(A));\
if(GTOKEN_RES != S_TOKEN_OK)\
{\
    return GTOKEN_RES;\
}

#define DELKAPRAVIDLA 10

#define SYN_OK 0
#define ID_NOT_DEFINED -1
#define ID_ALREADY_DEFINED -2
#define SYN_ERROR -3





int ExpRes(){
    tStack *local;
    local = (tStack*)malloc(sizeof(tStack));
}

//Semantic

//register new function with ID into sym table
int SEM_regFunc(char* name)
{
    return true;
}

//register new variable ID into sym table
int SEM_regId(char* name)
{
    return true;
}

//check if sym table contains ID
int SEM_existId(char* name)
{
    return true;
}

/*******
 *
 * Shora dolu
 *
 ****************/

int isType(Token *tok)
{
    if(tok->type != KEYWORD)
        return false;

    if(strcmp(tok->val, "integer") == 0){
        return true;
    }
    else if(strcmp(tok->val, "double") == 0){
        return true;
    }
    else if(strcmp(tok->val, "string") == 0){
        return true;
    }

    return false;
}

int SyntaxAnalyzer(){
    Token *act;
    int err;

    if(IN_FUCTION){
        return inFuction();
    }
    else{
        GET_TOKEN(act);
        if (act->type==KEYWORD){
            if(strcmp(act->val,"scope")==0){
                GET_TOKEN(act);
                if(act->type == EOL)
                {
                    IN_FUCTION = true;
                }
                else{
                    return SYN_ERROR;
                }
            }
            else if(strcmp(act->val,"declare")==0){
                    GET_TOKEN(act);
                    if(act->type==KEYWORD && strcmp(act->val,"function")==0){
                        S_decFunc();
                    }
            }
            else if(strcmp(act->val,"function")==0){
                    S_defFunc();
            }
        }
        else if(act->type==ID)
        {
            err=assigment(act);
        }
        else { return false;}
    }
    if (err==false){
        return false;
    }

    return 0;

}

int S_decFunc()
{
    Token *act;
    GET_TOKEN(act);
    if(act->type==ID){
        //TBD
        if(!SEM_regFunc(act->val))
        {
            return ID_ALREADY_DEFINED;
        }

        GET_TOKEN(act);

        if(act->type==LBRACKET)
        {
            return S_funcArgList(act, true);
        }

    }
    else
    {
        return SYN_ERROR;
    }

    return SYN_ERROR;
}

int S_defFunc()
{
    Token *act;
    GET_TOKEN(act);
    if(act->type==ID){
        if(!SEM_existId(act->val))
        {
            return ID_NOT_DEFINED;
        }

        GET_TOKEN(act);

        if(act->type==LBRACKET)
        {
            return S_funcArgList(act, false);
        }
    }
    else
    {
        return SYN_ERROR;
    }

    return SYN_ERROR;
}

int S_funcArgList(Token *act, bool declaration)
{
    GET_TOKEN(act);

    while(act->type!=RBRACKET){
        if(act->type == ID){
            //TBD function arguments in sym table
        }
        else{
            return SYN_ERROR;
        }

        GET_TOKEN(act);
        if(act->type != KEYWORD || strcmp(act->val, "as") != 0){
               return SYN_ERROR;
        }

        GET_TOKEN(act);
        if(!isType(act))
            return SYN_ERROR;

        //TBD function arguments in sym table

        GET_TOKEN(act);

        if(act->type != COMMA && act->type != RBRACKET){
            return SYN_ERROR;
        }

        GET_TOKEN(act);
    }

    return S_functype(declaration);
}

int S_functype(bool declaration)
{
    Token *act;
    GET_TOKEN(act);
    if(act->type != KEYWORD || strcmp(act->val, "as") != 0){
           return SYN_ERROR;
    }

    GET_TOKEN(act);
    if(!isType(act))
        return SYN_ERROR;

    //TBD function return type in sym table

    GET_TOKEN(act);
    if(act->type != EOL)
        return SYN_ERROR;

    IN_FUCTION = true;

    return SYN_OK;
}

int inFuction()
{
    Token *act;
    int err;

    GET_TOKEN(act);
    //printf("%s",act->val);
    if (act->type==KEYWORD){
        printf("nasel jsem klicove slovo");
        err=Keyword(act);
        printf("serious ? ");
    }
    else if(act->type==ID)
    {
        err=assigment(act);
    }
    else { return false;}
    if (err==false){
        return false;
    }
}



int assigment(Token *act)
{
    if(!SEM_existId(act->val)){
        return ID_NOT_DEFINED;
    }

    GET_TOKEN(act);

    if(act->type == EQL){
        GET_TOKEN(act);

        //check if fuction call and SEM check for declar
        //PreAnalyzer(act);
    }

    return SYN_OK;

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


int PrePosition(char c){

    switch(c){
        case 'i':
            return 0;
            break;
        case '+':
            return 1;
            break;
        case '-':
            return 2;
            break;
        case '*':
            return 3;
            break;
        case '(':
            return 4;
            break;
        case ')':
            return 5;
            break;
        case '$':
            return 6;
            break;
        case '/':
            return 7;
            break;
        case '\\':
            return 8;
            break;
        case '=':
            return 9;
            break;
        case '#':
            return 10;
            break;
        case ',':
            return 11;
            break;
        case '.':
            return 12;
            break;
        case '?':
            return 13;
            break;
        case ':':
            return 14;
            break;
        default:
            return -1;
    }

}

int DLListInitForParser(tDLList **local,tDLList **rule,tDLList **partrule){
    *rule = (tDLList*)malloc(sizeof(tDLList));
    if (*rule==NULL){
        return S_MEMORY_ERROR;
    }
    *partrule = (tDLList*)malloc(sizeof(tDLList));
    if (*partrule==NULL){
        return S_MEMORY_ERROR;
    }
    *local = (tDLList*)malloc(sizeof(tDLList));
    if (*local==NULL){
        return S_MEMORY_ERROR;
    }

    DLInitList(*rule);
    DLInitList(*partrule);
    DLInitList$(*local);

    return true;
}

int PreExe(char c, Token* act, tDLList* local,tDLList* partrule,tDLList* rule){
    char *top;
    int ret=0;
    Token *vys;

    top = (char*)malloc(sizeof(char));
    if (top==NULL){
        return S_MEMORY_ERROR;
    }




    do {
        DLCopyLastTerm(local, top);
        switch (ArtPreTB[PrePosition(*top)][ PrePosition(c)]) {
            case 3:  //=
                DLInsertLast(local, c, act);
                break;
            case 1: //<

              //  DLLast(local); // ukazu na nej
                DLLastTerm(local); //ukazu na nejlevejsi terminal
                DLInsertLast(local, c, act); //vlozim prvek na konec
                DLPostInsert(local, '<'); // vlozi za nej zarazku
                break;
            case 2: // >
                DLCutUntil(local, partrule);
                ret=3;

                DLDisposeList(rule);
                DLInsertLast(rule, 'E', NULL);
                DLInsertLast(rule, '+', NULL);
                DLInsertLast(rule, 'E', NULL);
                if(DLCompare(partrule, rule)==0){
                    printf("pravidlo pro uplatneni +\n");
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )
                    //gen
                    DLInsertLast(local,'E',vys);
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '*',NULL);
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){
                    printf("pravidlo pro uplatneni *\n");
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )
                    //gen
                    DLInsertLast(local,'E',vys);
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '-',NULL);
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){
                    printf("pravidlo pro uplatneni -\n");
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )
                    //gen
                    DLInsertLast(local,'E',vys);
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '/',NULL);
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){
                    printf("pravidlo pro uplatneni /\n");
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )
                    //gen
                    DLInsertLast(local,'E',vys);
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '\\',NULL);
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){
                    printf("pravidlo pro uplatneni \\ \n");
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )
                    //gen
                    DLInsertLast(local,'E',vys);
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '=',NULL);
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){
                    printf("pravidlo pro uplatneni =\n");
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )
                    //gen
                    DLInsertLast(local,'E',vys);
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '?',NULL);//<
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){
                    printf("pravidlo pro uplatneni <\n");
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )
                    //gen
                    DLInsertLast(local,'E',vys);
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, ':',NULL); //>
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){
                    printf("pravidlo pro uplatneni >\n");
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )

                    //gen
                    DLInsertLast(local,'E',&(vys));
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, ',',NULL); //<=
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){
                    printf("pravidlo pro uplatneni <=\n");
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )
                    //gen
                    DLInsertLast(local,'E',vys);
                    continue;
                } DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '.',NULL); //>=
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){
                    printf("pravidlo pro uplatneni >=\n");
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )

                    //gen
                    DLInsertLast(local,'E',vys);
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '#',NULL);
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){
                    printf("pravidlo pro uplatneni #\n");
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )

                    //gen
                    DLInsertLast(local,'E',vys);
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, '(',NULL);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, ')',NULL);
                if(DLCompare(partrule, rule)==0){
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )
                    printf("pravidlo pro uplatneni zavorek\n");
                    //gen
                    DLInsertLast(local,'E',vys);
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'i',NULL);
                if(DLCompare(partrule, rule)==0){
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )

                    //gen
                    DLInsertLast(local,'E',vys);
                    printf("uplatneni i->E \n");
                    continue;
                }
                // sem dojde pokud se yadne pravidlo neuplatni
                return 1;
                break;
            default:
                return 1; // chyba syntaxe

        }

    }while((local->Last->data!='$')&&(c=='$'));

    free(top);
    return ret;
}
/**
 * @brief funkce na kategorizaci tokenu pro Precedencni analyzu
 * @param act ukazatel na token
 * @param c ukazatel na char -> vystup
 * @return 1 v pripade nezadouciho tokenu 0-v pripade uspechu
 */
int PreTokenAnalyzer(Token *act, char * c){
    switch(act->type){
        case INTEGER:
        case DOUBLE:
        case ID:
        case STRING:
            *c='i';
            break;
        case DIV:
            *c='/';
            break;
        case MUL:
            *c='*';
            break;
        case ADD:
            *c='+';
            break;
        case SUB:
            *c='-';
            break;
        case MOD:
            *c='\\';
            break;
        case EQL:
            *c='=';
            break;
        case NEQL:
            *c='#';
            break;
        case LEQL:
            *c=','; //<=
            break;
        case GEQL:
            *c='.'; //>=
            break;
        case LT:
            *c='?'; // <
            break;
        case GT:
            *c=':'; // >
            break;
        case LBRACKET:
            *c='(';
            break;
        case RBRACKET:
            *c=')';
            break;
        default:
            return 1;
    }

    return 0;

}
Token* PreNextTok(Token* act, tDLList* local,tDLList* partrule,tDLList* rule){
    int err, ret;
    char *c;
    c=(char*)malloc(sizeof(char));
    if(c==NULL){
        return NULL;
    }

    do {
        if (act == NULL) {
            GET_TOKEN(act);
        }
        err = PreTokenAnalyzer(act, c);
        if (err == 1) {
            PreExe('$', NULL, local, partrule, rule);
        }
        else
        {
            ret=PreExe(*c, act,local,partrule,rule);
            if(ret==3){

            } else {
                act = NULL;
            }
        }
    }while(err!=1);

    free(c);
    c=NULL;

    return act;
}

 int PreAnalyzer(Token *act, Token**back){
     printf("vytejete v precedencni analyze \n");
     tDLList *local, *partrule, *rule;
     DLListInitForParser(&(local), &(rule), &(partrule));
     *back=PreNextTok(act, local, partrule, rule);
     DLDisposeList(local);
     DLDisposeList(partrule);
     DLDisposeList(rule);
     free(local);
     free(partrule);
     free(rule);
     local=NULL;
     partrule=NULL;
     rule=NULL;

}
