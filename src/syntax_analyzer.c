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
int S_StatList(Token *act, Token **back, bool isScope);
int S_Dim(Token *act);
int S_Input(Token *act);
int S_Print(Token *act);
int S_If(Token *act, bool isScope);
int S_While(Token *act, bool isScope);
int S_Ret(Token *act);
int S_Assig(Token *act);

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
char *FUNC;

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
#define SEM_OK 0
#define SYN_ERROR 2
#define SEM_ERROR 3
#define BIN_OP_INCOMPAT 4


//Semantic

//check if sym table contains ID
int SEM_existId(char* name)
{
    if(!Search_Func(name, NULL))
        if(!Search_Var(FUNC, name, NULL))
            return false;

    return true;
}

void setCurrFunc(char* name)
{
    free(FUNC);
    FUNC = malloc((strlen(name)+1) * sizeof(char));
    strcpy(FUNC, name);
}

void myStrCpy(char **to, char *from)
{
    free(*to);
    *to = malloc((strlen(from)+1) * sizeof(char));
    strcpy(*to, from);
}

int CheckRule(Token *op1, Token *oper, Token* op2, Token** res, TokType *typ1, TokType *typ2)
{
    (*res) = malloc(sizeof(Token));
    if(res == NULL){
        return S_MEMORY_ERROR;
    }

    TokType type1,type2;
    if(op1->type == ID){
        if(!Search_Var(FUNC, op1->val, &type1))
            return BIN_OP_INCOMPAT;
    }
    else if(op1->type == INTEGER){
        type1 = INTEGER;
    }
    else if(op1->type == DOUBLE){
        type1 = DOUBLE;
    }
    else if(op1->type == STRING){
        type1 = STRING;
    }
    else{
        return BIN_OP_INCOMPAT;
    }

    if(op2->type == ID){
        if(!Search_Var(FUNC, op2->val, &type2))
            return BIN_OP_INCOMPAT;
    }
    else if(op2->type == INTEGER){
        type2 = INTEGER;
    }
    else if(op2->type == DOUBLE){
        type2 = DOUBLE;
    }
    else if(op2->type == STRING){
        type2 = STRING;
    }
    else{
        return BIN_OP_INCOMPAT;
    }

    if(type1 == STRING && type2 == STRING){
        *typ1 = STRING;
        *typ2 = STRING;
        (*res)->type = STRING;
        return SEM_OK;
    }
    else if((type1 == DOUBLE || type2 == DOUBLE) && (type1 != STRING && type2 != STRING)){
        *typ1 = DOUBLE;
        *typ2 = DOUBLE;
        (*res)->type = DOUBLE;
        return SEM_OK;
    }
    else if(type1 == INTEGER && type2 == INTEGER){
        *typ1 = INTEGER;
        *typ2 = INTEGER;
        (*res)->type = INTEGER;
        return SEM_OK;
    }
    else{
        return BIN_OP_INCOMPAT;
    }

    switch(oper->type){
    case ADD:
        return SEM_OK;
        break;
    case SUB:
        return (*res)->type != STRING ? SEM_OK : BIN_OP_INCOMPAT ;
        break;
    case MUL:
        return (*res)->type != STRING ? SEM_OK : BIN_OP_INCOMPAT ;
        break;
    case DIV:
        if((*res)->type != STRING ){
            (*res)->type = DOUBLE;
            return SEM_OK;
        }
        else{
            return BIN_OP_INCOMPAT;
        }
        break;
    case IDIV:
        if((*res)->type != STRING){
            *typ1 = INTEGER;
            *typ2 = INTEGER;
            (*res)->type = INTEGER;
            return SEM_OK;
        }
        else{
            return BIN_OP_INCOMPAT;
        }
    default:
        break;
    }

    (*res)->type = BOOL;

    if((type1 == INTEGER && type2 == DOUBLE) || (type1 == DOUBLE && type2 == INTEGER)){
        type1 = DOUBLE;
        type2 = DOUBLE;
    }

    return SEM_OK;
}

int Checkid(Token *in, Token **out)
{
    TokType type;
    *out = malloc(sizeof(Token));
    if(out == NULL)
        return S_MEMORY_ERROR;

    if(in->type == ID){
        Search_Var(FUNC, in->val, &type);
    }
    else if(in->type == INTEGER){
        type = INTEGER;
    }
    else if(in->type == DOUBLE){
        type = DOUBLE;
    }
    else if(in->type == STRING){
        type = STRING;
    }
    else{
        return SEM_ERROR;
    }

    (*out)->type = type;

    return SEM_OK;
}


/*******
 *
 * Shora dolu
 *
 ****************/

bool isType(Token *tok, TokType *type)
{
    if(tok->type != KEYWORD)
        return false;

    if(strcmp(tok->val, "integer") == 0){
        if(type != NULL)
            *type = INTEGER;
        return true;
    }
    else if(strcmp(tok->val, "double") == 0){
        if(type != NULL)
            *type = DOUBLE;
        return true;
    }
    else if(strcmp(tok->val, "string") == 0){
        if(type != NULL)
            *type = STRING;
        return true;
    }

    return false;
}

int SyntaxAnalyzer(){
    Token *act,*back;
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
                    SYN_EXPAND(S_StatList,act,&back,true);

                    if(back->type != KEYWORD || strcmp(back->val, "end") != 0){
                       return SYN_ERROR;
                    }

                    GET_TOKEN(act);

                    if(act->type != KEYWORD)
                        return SYN_ERROR;
                    if((strcmp(act->val,"scope") != 0))
                        return SYN_ERROR;

                    GET_TOKEN(act);

                    if(act->type != EOL){
                        return SYN_ERROR;
                    }
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
                    else{
                        return SYN_ERROR;
                    }
            }
            else if(strcmp(act->val,"function")==0){
                    SYN_EXPAND(S_FuncHeader, false, act);
                    SYN_EXPAND(S_StatList,act,&back,false);

                    if(back->type != KEYWORD || strcmp(back->val, "end") != 0)
                       return SYN_ERROR;

                    GET_TOKEN(act);
                    if(act->type != KEYWORD)
                        return SYN_ERROR;
                    if((strcmp(act->val,"function") != 0))
                        return SYN_ERROR;

                    GET_TOKEN(act);
                    if(act->type != EOL)
                        return SYN_ERROR;
            }
        }
        else if(act->type == EOL){
            continue;
        }
        else{
                return SYN_ERROR;
        }
    }

    return 0;

}

int S_FuncHeader(bool declare, Token *act){

    bool definition = !declare;
    bool temp;
    //Function ID
    GET_TOKEN(act);

    if(act->type == ID){
        setCurrFunc(act->val);

        if(declare){
            if(Search_Func(FUNC, NULL))
                return SEM_ERROR;

            Dec_Func(FUNC, false);
        }
        else{
            if(!Search_Func(FUNC, &temp)){
                declare = true;
                Dec_Func(FUNC, true);
            }
            else{
                if(temp != false)
                    return SEM_ERROR;

                Define_Func(FUNC);
            }
        }
    }
    else{
        return SYN_ERROR;
    }

    //Function Arguments
    char *id = NULL;
    int i = 0;
    TokType type;

    GET_TOKEN(act);
    if(act->type != LBRACKET)
        return SYN_ERROR;

    while(act->type!=RBRACKET){
        GET_TOKEN(act);

        if(act->type == ID){
            myStrCpy(&id, act->val);
        }
        else{
            free(id);
            return SYN_ERROR;
        }

        if(SEM_existId(id)){
            free(id);
            return SEM_ERROR;
        }

        GET_TOKEN(act);
        if(act->type != KEYWORD || strcmp(act->val, "as") != 0){
            free(id);
            return SYN_ERROR;
        }

        GET_TOKEN(act);
        if(!isType(act, &type)){
            free(id);
            return SYN_ERROR;
        }

        if(declare)
            Dec_Func_AddArgument(FUNC, i, type);
        if(definition){
            Add_Var(FUNC, id, type);
        }
        else{
            free(id);
        }

        GET_TOKEN(act);

        if(act->type != COMMA && act->type != RBRACKET){
            free(id);
            return SYN_ERROR;
        }
        i++;
    }
    free(id);

    //Function Type
    GET_TOKEN(act);
    if(act->type != KEYWORD || strcmp(act->val, "as") != 0)
           return SYN_ERROR;

    GET_TOKEN(act);
    if(!isType(act, &type))
        return SYN_ERROR;

    Dec_Func_Set_Type(FUNC, type);

    GET_TOKEN(act);
    if(act->type != EOL)
        return SYN_ERROR;

    return SYN_OK;
}

int S_StatList(Token *act, Token **back, bool isScope){

    while(1){
        GET_TOKEN(act);
        if(act->type == KEYWORD){
            if(strcmp(act->val,"dim") == 0){
                SYN_EXPAND(S_Dim, act);
            }
            else if(strcmp(act->val,"input") == 0){
                SYN_EXPAND(S_Input, act);
            }
            else if(strcmp(act->val,"print") == 0){
                SYN_EXPAND(S_Print, act);
                continue;
            }
            else if(strcmp(act->val,"if") == 0){
                SYN_EXPAND(S_If, act, isScope);
            }
            else if(strcmp(act->val,"do") == 0){
                GET_TOKEN(act);
                if(act->type == KEYWORD && strcmp(act->val,"while") == 0){
                    SYN_EXPAND(S_While, act, isScope);
                }
                else{
                    return SYN_ERROR;
                }
            }
            else if(strcmp(act->val,"return") == 0 && !isScope){
                SYN_EXPAND(S_Ret, act);
                continue;
            }
            else if(strcmp(act->val,"end") == 0 || strcmp(act->val,"loop") == 0 || strcmp(act->val,"else") == 0){
                *back = act;
                return SYN_OK;
            }
            else{
                return SYN_ERROR;
            }

            GET_TOKEN(act);
            if(act->type != EOL)
                return SYN_ERROR;
        }
        else if(act->type == ID){
            char *id = NULL;
            myStrCpy(&id, act->val);
            if(!SEM_existId(act->val)){
                free(id);
                return SEM_ERROR;
            }

            GET_TOKEN(act);
            if(act->type == EQL){
                SYN_EXPAND(S_Assig, act);
            }
            else{
                free(id);
                return SYN_ERROR;
            }

            getResult(id, false);
            free(id);
        }
        else if(act->type == EOL){
            continue;
        }
        else{
            return SYN_ERROR;
        }
    }


    return SYN_OK;
}

int S_Dim(Token *act){
    char *id;
    TokType type;

    GET_TOKEN(act);
    if(act->type == ID){
        myStrCpy(&id, act->val);
    }
    else{
        return SYN_ERROR;
    }

    GET_TOKEN(act);
    if(act->type != KEYWORD || strcmp(act->val, "as") != 0)
           return SYN_ERROR;

    GET_TOKEN(act);
    if(!isType(act, &type))
        return SYN_ERROR;

    if(SEM_existId(id)){
        free(id);
        return SEM_ERROR;
    }

    Add_Var(FUNC, id, type);
    variableDeclaration(id);

    return SYN_OK;
}

int S_Input(Token *act){
    GET_TOKEN(act);
    if(act->type == ID){
        if(!Search_Var(FUNC, act->val, NULL))
            return SEM_ERROR;
    }
    else{
        return SYN_ERROR;
    }

    TokType type;
    Search_Var(FUNC, act->val, &type);
    input(act->val, type);

    return SYN_OK;
}

int S_Print(Token *act){
    Token *back;

    do{
        GET_TOKEN(act);
        PreAnalyzer(act, &back);
    }while(back->type == SEMICOLON);

    write();

    if(back->type != EOL)
        return SYN_ERROR;

    return SYN_OK;
}

int S_If(Token *act, bool isScope){
    Token *back;

    GET_TOKEN(act);
    PreAnalyzer(act, &back);

    if(back->type != KEYWORD || strcmp(back->val, "then") != 0)
           return SYN_ERROR;

    GET_TOKEN(act);
    if(act->type != EOL)
           return SYN_ERROR;

    SYN_EXPAND(S_StatList, act, &back, isScope);

    if(back->type != KEYWORD || strcmp(back->val, "else") != 0)
           return SYN_ERROR;

    GET_TOKEN(act);
    if(act->type != EOL)
           return SYN_ERROR;

    SYN_EXPAND(S_StatList, act, &back, isScope);

    if(back->type != KEYWORD || strcmp(back->val, "end") != 0)
       return SYN_ERROR;

    GET_TOKEN(act);
    if(act->type != KEYWORD || strcmp(act->val, "if") != 0)
           return SYN_ERROR;

    return SYN_OK;
}

int S_While(Token *act, bool isScope){
    Token *back;

    GET_TOKEN(act);
    PreAnalyzer(act, &back);

    if(back->type != EOL){
           return SYN_ERROR;
    }

    SYN_EXPAND(S_StatList, act, &back, isScope);

    if(back->type != KEYWORD || strcmp(back->val, "loop") != 0){
       return SYN_ERROR;
    }

    return SYN_OK;
}

int S_Ret(Token *act){
    Token *back;

    GET_TOKEN(act);
    PreAnalyzer(act, &back);

    if(back->type != EOL)
           return SYN_ERROR;

    return SYN_OK;
}

int S_Assig(Token *act){
    Token *back;
    TokType type;

    GET_TOKEN(act);
    if(act->type == ID){
        //if func - params else preanalyzer
        if(Search_Func(act->val, NULL)){
            char *id;
            myStrCpy(&id, act->val);

            GET_TOKEN(act);
            if(act->type != LBRACKET)
                return SYN_ERROR;

            int i = 0;
            while(act->type!=RBRACKET){
                GET_TOKEN(act);

                if(act->type == ID){
                    if(!Search_Var(FUNC, act->val, &type)){
                        free(id);
                        return SEM_ERROR;
                    }
                    if(!Nth_Func_ArgType(id, i, type)){
                        free(id);
                        return SEM_ERROR;
                    }
                }
                else if(act->type == INTEGER){
                    if(!Nth_Func_ArgType(id, i, INTEGER)){
                        free(id);
                        return SEM_ERROR;
                    }
                }
                else if(act->type == DOUBLE){
                    if(!Nth_Func_ArgType(id, i, DOUBLE)){
                        free(id);
                        return SEM_ERROR;
                    }
                }
                else if(act->type == STRING){
                    if(!Nth_Func_ArgType(id, i, STRING)){
                        free(id);
                        return SEM_ERROR;
                    }
                }
                else{
                    free(id);
                    return SYN_ERROR;
                }

                GET_TOKEN(act);

                if(act->type != COMMA && act->type != RBRACKET){
                    free(id);
                    return SYN_ERROR;
                }
                i++;
            }
            free(id);
        }
        else{
            PreAnalyzer(act, &back);
            if(back->type != EOL)
                return SYN_ERROR;
        }
    }
    else{
        PreAnalyzer(act, &back);
        if(back->type != EOL)
            return SYN_ERROR;
    }

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

                    // PROSIM UPRAVTE ZDE PARAMETRY VKLADANE DO FCE A ODKOMENTUJTE operationSelect('+',BOOL,TOKTYPE);
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

                    // PROSIM UPRAVTE ZDE PARAMETRY VKLADANE DO FCE A ODKOMENTUJTE operationSelect('*',BOOL,TOKTYPE);
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

                    // PROSIM UPRAVTE ZDE PARAMETRY VKLADANE DO FCE A ODKOMENTUJTE operationSelect('-',BOOL,TOKTYPE);
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

                    // PROSIM UPRAVTE ZDE PARAMETRY VKLADANE DO FCE A ODKOMENTUJTE operationSelect('/',BOOL,TOKTYPE);
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
                    // PROSIM UPRAVTE ZDE PARAMETRY VKLADANE DO FCE A ODKOMENTUJTE operationSelect('\\',BOOL,TOKTYPE);
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

                    DLInsertLast(local,'E',vys);
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'i',NULL);
                if(DLCompare(partrule, rule)==0){
                    //martinova fce(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys) )

                    //getOperand(partrule->First->act, isVariable, type);
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
        case IDIV:
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
