#include "syntax_analyzer.h"
#include "symtable.h"
#include "string.h"
#include "stdbool.h"
#include "DLlist.h"
#include "stack_operations.h"
#include "code_gen.h"

/**
*
*
*
*
*
*
**/

int S_FuncHeader(bool declare, Token *act);
int S_StatList(bool scope, Token *act);

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

int RET_VAL;

#define GET_TOKEN(A)\
RET_VAL = GetToken(&(A));\
if(RET_VAL != S_TOKEN_OK)\
{\
    return RET_VAL;\
}

#define SYN_EXPAND(A, ...)\
RET_VAL = (A)(__VA_ARGS__);\
if(RET_VAL != SYN_OK)\
{\
  return RET_VAL;\
}\

#define DELKAPRAVIDLA 10

#define SYN_OK 0
#define ID_NOT_DEFINED -1
#define ID_ALREADY_DEFINED -2
#define SYN_ERROR -3





int ExpRes(){

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

bool isType(Token *tok)
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
    bool scope = false;

    while(1){
        GET_TOKEN(act);
        if (act->type==KEYWORD){
            if(strcmp(act->val,"scope")==0){
                if(scope){
                    return SYN_ERROR;
                }

                GET_TOKEN(act);
                if(act->type == EOL)
                {
                    scope = true;
                    //retVal = S_StatList(true);

                    SYN_EXPAND(S_StatList,true,act);
                }
                else{
                    return SYN_ERROR;
                }
            }
            else if(strcmp(act->val,"declare")==0){
                    GET_TOKEN(act);
                    if(act->type==KEYWORD && strcmp(act->val,"function")==0){
                        SYN_EXPAND(S_FuncHeader, true, act);
                    }
            }
            else if(strcmp(act->val,"function")==0){
                    SYN_EXPAND(S_FuncHeader, false, act);
            }
        }
        else{
                return SYN_ERROR;
        }
    }
}

int S_FuncHeader(bool declare, Token *act){
    //Function ID
    GET_TOKEN(act);
    if(act->type == ID){
        //TBD function arguments in sym table
        if(declare){

        }
        else{

        }
    }
    else{
        return SYN_ERROR;
    }

    //Function Arguments
    char *id;

    GET_TOKEN(act);
    while(act->type!=RBRACKET){
        if(act->type == ID){
            id = act->val;
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
        if(declare){

        }
        else{

        }

        GET_TOKEN(act);

        if(act->type != COMMA && act->type != RBRACKET){
            return SYN_ERROR;
        }

        GET_TOKEN(act);
    }

    //Function Type
    GET_TOKEN(act);
    if(act->type != KEYWORD || strcmp(act->val, "as") != 0){
           return SYN_ERROR;
    }

    GET_TOKEN(act);
    if(!isType(act))
        return SYN_ERROR;

    //TBD function return type in sym table
    if(declare){

    }
    else{

    }

    GET_TOKEN(act);
    if(act->type != EOL)
        return SYN_ERROR;

    if(!declare){
        SYN_EXPAND(S_StatList, false, act);
    }

    return SYN_OK;
}

int S_StatList(bool scope, Token *act){
    return SYN_OK;
}



//zdola nahoru


int PrePosition(char c){

    switch(c){
        case 'i':
            return 0;
        case '+':
            return 1;
        case '-':
            return 2;
        case '*':
            return 3;
        case '(':
            return 4;
        case ')':
            return 5;
        case '$':
            return 6;
        case '/':
            return 7;
        case '\\':
            return 8;
        case '=':
            return 9;
        case '#':
            return 10;
        case ',':
            return 11;
        case '.':
            return 12;
        case '?':
            return 13;
        case ':':
            return 14;
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
                    value(partrule->First->act);
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
            getResult();
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
     TFCreation();
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
