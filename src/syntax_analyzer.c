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
int S_Assig(Token *act, bool *function);

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

#define  FREEPARTRULE()\
FreeToken(&(partrule->First->act));\
FreeToken(&(partrule->First->rptr->act));\
FreeToken(&(partrule->First->rptr->rptr->act));\

#define CHECKRULE()\
retid=CheckRule(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys), &type1, &type2);\
if (retid != 0){\
return retid;\
}\
if((partrule->First->act->type != type1) || (partrule->First->rptr->rptr->act->type != type2)){\
changed = true;\
}\
else{\
changed = false;\
}\

#define  CHECKBOOL()\
retid=CheckRule(partrule->First->act, partrule->First->rptr->act, partrule->First->rptr->rptr->act, &(vys), &type1, &type2);\
if (retid!=0){\
return retid;\
}\

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
        if(!Search_Var(FUNC, in->val, &type)){
            return SEM_ERROR;
        }
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


    Dec_Func("length", true);
    Dec_Func("substr", true);
    Dec_Func("asc", true);
    Dec_Func("chr", true);

    Dec_Func_AddArgument("length", 0, STRING);

    Dec_Func_AddArgument("substr", 0, STRING);
    Dec_Func_AddArgument("substr", 1, INTEGER);
    Dec_Func_AddArgument("substr", 2, INTEGER);

    Dec_Func_AddArgument("asc", 0, STRING);
    Dec_Func_AddArgument("asc", 1, INTEGER);

    Dec_Func_AddArgument("chr", 0, INTEGER);

    Dec_Func_Set_Type("length", INTEGER);
    Dec_Func_Set_Type("substr", STRING);
    Dec_Func_Set_Type("asc", INTEGER);
    Dec_Func_Set_Type("chr", STRING);

    //Code Gen - Init
    header();

    while(1){
        GET_TOKEN(act);

        if (act->type==KEYWORD && !scope){
            if(strcmp(act->val,"scope")==0){
                if(scope){
                    return SYN_ERROR;
                }

                free(FUNC);
                scopeLabel();
                Dec_Func("scope", true);
                setCurrFunc("scope");

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

                    functionReturn(false);
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
                if(temp)
                    return SEM_ERROR;

                Define_Func(FUNC);
            }
        }
    }
    else{
        return SYN_ERROR;
    }

    //Code Gen - Definition
    if(definition)
        functionDefinition(FUNC);

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

        //Code Gen - parameter
        if(definition)
            functionParamLoad(*act);

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

            bool func;
            GET_TOKEN(act);
            if(act->type == EQL){
                SYN_EXPAND(S_Assig, act, &func);
            }
            else{
                free(id);
                return SYN_ERROR;
            }

            getResult(id, func);
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
    char *id = NULL;
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
    TokType type;

    do{
        GET_TOKEN(act);

        if(act->type == EOL)
            break;

        PreAnalyzer(act, &back, &type);
        write();
    }while(back->type == SEMICOLON);


    if(back->type != EOL && act->type != EOL)
        return SYN_ERROR;

    return SYN_OK;
}

int S_If(Token *act, bool isScope){
    Token *back;
    TokType type;

    whileIfBegin(IF);

    GET_TOKEN(act);
    PreAnalyzer(act, &back, &type);

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

    whileIfElseEnd(ELSE);

    SYN_EXPAND(S_StatList, act, &back, isScope);

    if(back->type != KEYWORD || strcmp(back->val, "end") != 0)
       return SYN_ERROR;

    GET_TOKEN(act);
    if(act->type != KEYWORD || strcmp(act->val, "if") != 0)
           return SYN_ERROR;

    whileIfElseEnd(IF);

    return SYN_OK;
}

int S_While(Token *act, bool isScope){
    Token *back;
    TokType type;

    whileIfBegin(WHILE);

    GET_TOKEN(act);
    PreAnalyzer(act, &back, &type);

    if(back->type != EOL){
           return SYN_ERROR;
    }

    SYN_EXPAND(S_StatList, act, &back, isScope);

    if(back->type != KEYWORD || strcmp(back->val, "loop") != 0){
       return SYN_ERROR;
    }

    whileIfElseEnd(WHILE);

    return SYN_OK;
}

int S_Ret(Token *act){
    Token *back;
    TokType type;

    GET_TOKEN(act);
    PreAnalyzer(act, &back, &type);

    functionReturn(true);

    if(back->type != EOL)
           return SYN_ERROR;

    return SYN_OK;
}

int S_Assig(Token *act, bool *function){
    Token *back;
    TokType type;

    GET_TOKEN(act);
    if(act->type == ID || act->type == KEYWORD){
        //if func - params else preanalyzer
        if(Search_Func(act->val, NULL)){
            //Code Gen - f. call init
            functionFramePreparation();

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

                //Code Gen - Load parameter
                callParamLoad(*act);

                GET_TOKEN(act);

                if(act->type != COMMA && act->type != RBRACKET){
                    free(id);
                    return SYN_ERROR;
                }
                i++;
            }
            //Code Gen
            callParamsPush();

            //Code Gen - actual function call
            callInstruction(id);

            *function = true;

            free(id);
        }
        else{
            PreAnalyzer(act, &back, &type);
            if(back->type != EOL)
                return SYN_ERROR;

            *function = false;
        }
    }
    else{
        PreAnalyzer(act, &back, &type);
        if(back->type != EOL)
            return SYN_ERROR;

        *function = false;
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
/**
 * @brief funkce vykonavajici precedencni analyzu
 * @param c
 * @param act
 * @param local
 * @param partrule
 * @param rule
 * @return
 */
int PreExe(char c, Token* act, tDLList* local,tDLList* partrule,tDLList* rule){
    char *top;
    int ret=0, retid=0;
    bool changed;
    Token *vys;
    TokType type1, type2;

    top = (char*)malloc(sizeof(char));
    if (top==NULL){
        return 99;
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
                ret=7;

                DLDisposeList(rule);
                DLInsertLast(rule, 'E', NULL);
                DLInsertLast(rule, '+', NULL);
                DLInsertLast(rule, 'E', NULL);
                if(DLCompare(partrule, rule)==0){

                    CHECKRULE();

                    operationSelect('+', changed, vys->type);

                    DLInsertLast(local,'E',vys);
                    FREEPARTRULE();
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '*',NULL);
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){

                    CHECKRULE();

                    operationSelect('*', changed, vys->type);

                    DLInsertLast(local,'E',vys);
                    FREEPARTRULE();
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '-',NULL);
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){

                   CHECKRULE();

                    operationSelect('-', changed, vys->type);
                    DLInsertLast(local,'E',vys);
                    FREEPARTRULE();
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '/',NULL);
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){

                    CHECKRULE();

                    operationSelect('/', changed, vys->type);

                    DLInsertLast(local,'E',vys);
                    FREEPARTRULE();
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '\\',NULL);
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){

                    CHECKRULE();

                    operationSelect('\\', changed, vys->type);

                    DLInsertLast(local,'E',vys);
                    FREEPARTRULE();
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '=',NULL);
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){

                    CHECKBOOL();
                    boolOperationSelect('=', type1, type2);
                    //gen
                    DLInsertLast(local,'E',vys);
                    FREEPARTRULE();
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '?',NULL);//<
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){

                    CHECKBOOL();

                    boolOperationSelect('?', type1, type2);

                    DLInsertLast(local,'E',vys);
                    FREEPARTRULE();
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, ':',NULL); //>
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){

                    CHECKBOOL();

                    boolOperationSelect(':', type1, type2);



                    DLInsertLast(local,'E',vys);
                    FREEPARTRULE();
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, ',',NULL); //<=
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){

                    CHECKBOOL();

                    boolOperationSelect(',', type1, type2);

                    //gen
                    DLInsertLast(local,'E',vys);
                    FREEPARTRULE();
                    continue;
                } DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '.',NULL); //>=
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){

                    CHECKBOOL();

                    boolOperationSelect('.', type1, type2);
                    DLInsertLast(local,'E',vys);
                    FREEPARTRULE();
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, '#',NULL);
                DLInsertLast(rule, 'E',NULL);
                if(DLCompare(partrule, rule)==0){

                    CHECKBOOL();

                    boolOperationSelect('#', type1, type2);
                    //gen
                    DLInsertLast(local,'E',vys);
                    FREEPARTRULE();
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, '(',NULL);
                DLInsertLast(rule, 'E',NULL);
                DLInsertLast(rule, ')',NULL);
                if(DLCompare(partrule, rule)==0){

                    DLInsertLast(local,'E',vys);
                    FREEPARTRULE();
                    continue;
                }
                DLDisposeList(rule);
                DLInsertLast(rule, 'i',NULL);
                if(DLCompare(partrule, rule)==0){
                    retid=Checkid(partrule->First->act,&vys);
                    if(retid != 0)
                    {
                        return retid;
                    }

                    getOperand(partrule->First->act);
                    DLInsertLast(local,'E',vys);

                    continue;
                }
                // sem dojde pokud se zadne pravidlo neuplatni
                return 2;
                break;
            default:
                return 2; // chyba syntaxe

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
/**
 * @brief funkce ridici precedencni analyzu a zaroven ziskavaji dalsi token
 * @param act aktualni token popripade predany
 * @param local DLlist je hlavni zasobnik pro prec analyzu
 * @param partrule DLList pro vyriznuti casti zasobniku a zpraocvani pravidel
 * @param rule  DLList pravidlo ktere se porovnava obsahuje postupne vsechny pravidla gramatiky pro precedencni analyzu
 * @param vys DLList vysledek ze sematiky a postupne predavany DLListem
 * @return v pripade chyby vraci prislusnou chybu jinak 0
 */
int PreNextTok(Token* act, tDLList* local,tDLList* partrule,tDLList* rule, Token** vys){
    int err, tok;
    char *c;

    c=(char*)malloc(sizeof(char));
    if(c==NULL){
        *vys=NULL;
        return 99;
    }

    do {
        if (act == NULL) { //umoznuje zpracovavat jeden token vicekrat
            GET_TOKEN(act);
        }
        tok = PreTokenAnalyzer(act, c); //overeni zda token je validni pro precedencni analyzu
        if (tok == 1) { //kdyz token nepotri ukoncujeme prec analyzu
            PreExe('$', NULL, local, partrule, rule); // posleme ukoncovaci znak
        }
        else // novy nebo stavajici token
        {
            err=PreExe(*c, act,local,partrule,rule);
            if(err==7){ // pokud neprobehlo zpracovani stavajiciho tokenu TOTO NENI CHYBA

            }
            else if(err==0){
                act = NULL; // zpracovano, chceme dalsi token
            }
            else {
                return err; // nejaka chyba
            }
        }
    }while(tok!=1);

    free(c);
    c=NULL;

    *vys=act;
    return 0;
}
/**
 * @brief hlavni fce pro precedencni analyzu
 * @param act token ktery jiz patri precedenci
 * @param back token vraceny token ktery nepatri precedencni analyze
 * @param res vysledek posledni vrchol simulovaneho derivacniho stromu
 * @return v pripade chyby vraci jeji cislo jinak 0
 */
 int PreAnalyzer(Token *act, Token**back, TokType* res){

     tDLList *local, *partrule, *rule;
     Token* vys;
     int err;

     TFCreation();   // blok inicializaci
     DLListInitForParser(&(local), &(rule), &(partrule));

     err=PreNextTok(act, local, partrule, rule, &(vys)); // hlavni funkce ridici precedencni analyzu
     *res=local->First->rptr->act->type;
     *back=vys;

     DLDisposeList(local);  //cisteni
     DLDisposeList(partrule);
     DLDisposeList(rule);

     free(local);
     free(partrule);
     free(rule);

     local=NULL;
     partrule=NULL;
     rule=NULL;

     return err; // vraceni pripadnych chyb
}
