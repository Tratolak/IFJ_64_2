#include <stdio.h>
#include "code_gen.h"

//Martin Stod�lka(xstodu08)
//Ond�ej Ol��k(xolsak00)
//Michael Schneider(xschne07)
//Marek Kuchynka(xkuchy00)


//Pocitani poctu navesti daneho typu
int whileLabelQuantity = 0;
int ifLabelQuantity = 0;
int variableQuantity;

//================================================================================
//
//================================================================================

/**
 * Utvoreni povinne hlavicky IFJcode17, inicializace zasobniku pro navesti.
 * Instrukce pro skok na navesti zacatku programu.
 */
void header() {
    printf(".IFJcode17\n");
    labelStackInit(&labelStack);
    printf("CREATEFRAME\n");
    printf("JUMP scope\n");
    inBuiltLength();
    inBuiltSubStr();
    inBuiltAsc();
    inBuiltChr();
}

//================================================================================
// Funkce potrebne pro generovani instrukci na vyhodnoceni vyrazu
//================================================================================


/**
 * Vytvoreni docasneho ramce pro vypocet vyrazu. A inicializace zasobniku typu.
 */
void TFCreation() {
    typeStackInit(&typeStack);
    printf("PUSHFRAME\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@_exprOperand1\n");
    printf("DEFVAR TF@_exprOperand2\n");
    printf("DEFVAR TF@_exprResult\n");
}

/**
 * Funkse zjisti zda-li je treba provest pretypovani nektere z hodnot a zavola fci,
 * ktera vygeneruje instrukci na dany prevod.
 *
 * @brief exprOperand1 == first & exprOperand2 == second (rovnost hodnot na zasobnicich)
 *
 * @param convert - urcuje zda-li je pretypovani potrebne (bool)
 * @param type    - urcuje typ vysledku (TokType)
 * @return status - informace o uspesnosti alokace (int)
 */
int typeConvert(bool convert, TokType type) {
    printf("POPS TF@_exprOperand1\n"); //exprOperand1 == first
    printf("POPS TF@_exprOperand2\n"); //exprOperand2 == second

    TokType first, second;
    typeStackPop(&typeStack, &first);
    typeStackPop(&typeStack, &second);

    if (convert) {
        if (first != type)
            convertInstructionSelect(first, type, "TF@_exprOperand1");
        if (second != type)
            convertInstructionSelect(second, type, "TF@_exprOperand2");
    }
    int status = typeStackPush(&typeStack, type);
    return status;
}

/**
 * Vygeneruje instrukci na pretypovani v zavislosti na vstupnich parametrech.
 *
 * @param original - puvodni typ promene (TokType)
 * @param new      - novy typ promenne (TokType)
 * @param string   - nazev promenne se kterou se pracuje v IFJcode17 (string)
 */
void convertInstructionSelect(TokType original, TokType new, char *string) {
    if (original == INTEGER && new == DOUBLE) {
        printf("INT2FLOAT %s %s\n", string, string);
    } else if (original == DOUBLE && new == INTEGER) {
        printf("FLOAT2INT %s %s\n", string, string);
    } else if (original == INTEGER && new == STRING) {
        printf("INT2CHAR %s %s\n", string, string);
    }
}

/**
 * Ziskani poslednich dvou hodnot ulozenych na zasobniku, provedeni operace
 * dle operand a ulozeni vysledku na vrchol zasobniku.
 *
 * @param operation - provadena operace (char)
 * @param convert   - true = je treba pretypovat | false = neni treba pretypovat (bool)
 * @param type      - typ do ktereho se provede pripadne pretypovani (TokType)
 */
int operationSelect(char operand, bool convert, TokType type) {
    int status = typeConvert(convert, type);
    switch (operand) {
        case '+':
            if (type == STRING) {
                printf("CONCAT TF@_exprResult TF@_exprOperand2 TF@_exprOperand1\n");
                printf("PUSHS TF@_exprResult\n");
            } else {
                printf("ADD TF@_exprResult TF@_exprOperand2 TF@_exprOperand1\n");
                printf("PUSHS TF@_exprResult\n");
            }
            break;
        case '-':
            printf("SUB TF@_exprResult TF@_exprOperand2 TF@_exprOperand1\n");
            printf("PUSHS TF@_exprResult\n");
            break;
        case '*':
            printf("MUL TF@_exprResult TF@_exprOperand2 TF@_exprOperand1\n");
            printf("PUSHS TF@_exprResult\n");
            break;
        case '/': //Vysledek je vzdy double
            printf("DIV TF@_exprResult TF@_exprOperand2 TF@_exprOperand1\n");
            if (type != DOUBLE) {
                convertInstructionSelect(type, DOUBLE, "TF@_exprResult");
            }
            printf("PUSHS TF@_exprResult\n");
            break;
        case '\\': //Operandy a vysledek jsou typu INTEGER
            printf("DIV TF@_exprResult TF@_exprOperand2 TF@_exprOperand1\n");
            printf("PUSHS TF@_exprResult\n");
            break;
        default:
            break;
    }
    return status;
}

/**
 * Generovani instrukci pro provedeni konverze.
 *
 * @param var1 - typ prvni promenne (TokType)
 * @param var2 - typ druhe promenne (TokType)
 */
void boolOperationConvert(TokType var1, TokType var2) {
    printf("POPS TF@_exprOperand1\n"); //exprOperand1 == var2
    printf("POPS TF@_exprOperand2\n"); //exprOperand2 == var1

    TokType first, second;
    typeStackPop(&typeStack, &first);
    typeStackPop(&typeStack, &second);
    if (var1 != var2) {
        if (var1 == STRING || var2 == STRING) {
            if (var2 != STRING) {
                convertInstructionSelect(var2, STRING, "TF@_exprOperand1");
            } else if (var1 != STRING) {
                convertInstructionSelect(var1, STRING, "TF@_exprOperand2");
            }
        } else if (var1 == DOUBLE || var2 == DOUBLE) {
            if (var2 != DOUBLE) {
                convertInstructionSelect(var2, DOUBLE, "TF@_exprOperand1");
            } else if (var1 != DOUBLE) {
                convertInstructionSelect(var1, DOUBLE, "TF@_exprOperand2");
            }
        }
    }
}

/**
 * Generovani vyrazu pro konstrukce IF,IF ELSE a WHILE.
 *
 * @param operation - provadena operace (char)
 * @param convert   - true = je treba pretypovat | false = neni treba pretypovat (bool)
 * @param type      - typ do ktereho se provede pripadne pretypovani (TokType)
 */
void boolOperationSelect(char operand, TokType var1, TokType var2) {
    boolOperationConvert(var1, var2);
    labelType lType;
    int quantity;

    switch (operand) {
        case '=':
            printf("PUSHS TF@_exprOperand2\n");
            printf("PUSHS TF@_exprOperand1\n");
            printf("EQS\n");
            printf("PUSHS bool@true\n");
            labelStackTop(&labelStack, &lType, &quantity);
            if (lType == WHILE) {
                printf("POPFRAME\n");
                printf("JUMPIFNEQS $$while%dEnd\n", quantity); //while%dEnd - navesti konce cyklu
            } else if (lType == IF) {
                printf("POPFRAME\n");
                printf("JUMPIFNEQS $$else%d\n", quantity); //if%dFalse - zacatek false vetve podminky
            }
            break;
        case '#':  //!=
            printf("PUSHS TF@_exprOperand2\n");
            printf("PUSHS TF@_exprOperand1\n");
            printf("EQS\n");
            printf("PUSHS bool@false\n");
            labelStackTop(&labelStack, &lType, &quantity);
            if (lType == WHILE) {
                printf("POPFRAME\n");
                printf("JUMPIFNEQS $$while%dEnd\n", quantity); //while%dEnd - navesti konce cyklu
            } else if (lType == IF) {
                printf("POPFRAME\n");
                printf("JUMPIFNEQS $$else%d\n", quantity); //if%dFalse - zacatek false vetve podminky
            }
            break;
        case ',': //<=
            printf("PUSHS TF@_exprOperand2\n");
            printf("PUSHS TF@_exprOperand1\n");
            printf("EQS\n");
            printf("PUSHS TF@_exprOperand2\n");
            printf("PUSHS TF@_exprOperand1\n");
            printf("LTS\n");
            printf("ORS\n");
            printf("PUSHS bool@true\n");
            labelStackTop(&labelStack, &lType, &quantity);
            if (lType == WHILE) {
                printf("POPFRAME\n");
                printf("JUMPIFNEQS $$while%dEnd\n", quantity); //while%dEnd - navesti konce cyklu
            } else if (lType == IF) {
                printf("POPFRAME\n");
                printf("JUMPIFNEQS $$else%d\n", quantity); //if%dFalse - zacatek false vetve podminky
            }
            break;
        case '.': //>=
            printf("PUSHS TF@_exprOperand2\n");
            printf("PUSHS TF@_exprOperand1\n");
            printf("EQS\n");
            printf("PUSHS TF@_exprOperand2\n");
            printf("PUSHS TF@_exprOperand1\n");
            printf("GTS\n");
            printf("ORS\n");
            printf("PUSHS bool@true\n");
            labelStackTop(&labelStack, &lType, &quantity);
            if (lType == WHILE) {
                printf("POPFRAME\n");
                printf("JUMPIFNEQS $$while%dEnd\n", quantity); //while%dEnd - navesti konce cyklu
            } else if (lType == IF) {
                printf("POPFRAME\n");
                printf("JUMPIFNEQS $$else%d\n", quantity); //if%dFalse - zacatek false vetve podminky
            }
            break;
        case '?': // <
            printf("PUSHS TF@_exprOperand2\n");
            printf("PUSHS TF@_exprOperand1\n");
            printf("LTS\n");
            printf("PUSHS bool@true\n");
            labelStackTop(&labelStack, &lType, &quantity);
            if (lType == WHILE) {
                printf("POPFRAME\n");
                printf("JUMPIFNEQS $$while%dEnd\n", quantity); //while%dEnd - navesti konce cyklu
            } else if (lType == IF) {
                printf("POPFRAME\n");
                printf("JUMPIFNEQS $$else%d\n", quantity); //if%dFalse - zacatek false vetve podminky
            }
            break;
        case ':': //>
            printf("PUSHS TF@_exprOperand2\n");
            printf("PUSHS TF@_exprOperand1\n");
            printf("GTS\n");
            printf("PUSHS bool@true\n");
            labelStackTop(&labelStack, &lType, &quantity);
            if (lType == WHILE) {
                printf("POPFRAME\n");
                printf("JUMPIFNEQS $$while%dEnd\n", quantity); //while%dEnd - navesti konce cyklu
            } else if (lType == IF) {
                printf("POPFRAME\n");
                printf("JUMPIFNEQS $$else%d\n", quantity); //if%dFalse - zacatek false vetve podminky
            }
        default:
            break;
    }
}

/**
 * Ulozeni hodnoty/promenne na vrchol zasobniku.
 *
 * @param t          - promenna/absolutni hodnota (Token)
 * @param isVariable - true jedna-li se o promennou (bool)
 * @param type       - typ promenne (TokType)
 * @return status    -  informace o uspesnosti alokace (int)
 */
int getOperand(Token *t) {
    int status = STATUS_OK;
    if (t->type == INTEGER) {
        printf("PUSHS int@%s\n", t->val);
        status = typeStackPush(&typeStack, t->type);
    } else if (t->type == DOUBLE) {
        printf("PUSHS float@%s\n", t->val);
        status = typeStackPush(&typeStack, t->type);
    } else if (t->type == STRING) {
        printf("PUSHS string@%s\n", t->val);
        status = typeStackPush(&typeStack, t->type);
    } else if (t->type == ID) {
        printf("PUSHS LF@_%s\n", t->val);
        status = typeStackPush(&typeStack, t->type);
    }
    return status;
}

/**
 * Generovani instrukci pro pretypovani vysledku vyrazu.
 * @brief Je treba tuhle fci volat jeste pred funkcemi getRestult() a functionReturn().
 *
 * @param original - aktualni typ promenne (TokType)
 * @param new      - vysledny typ promenne (TokType)
 */
void retype(TokType original, TokType new){
    printf("POPS TF@_exprResult\n");
    if (original == INTEGER && new == DOUBLE) {
        printf("INT2FLOAT TF@_exprResult TF@_exprResult\n");
    } else if (original == DOUBLE && new == INTEGER) {
        printf("FLOAT2INT TF@_exprResult TF@_exprResult\n");
    } else if (original == INTEGER && new == STRING) {
        printf("INT2CHAR TF@_exprResult TF@_exprResult\n");
    }
    printf("PUSHS TF@_exprResult\n");
}

/**
 * Generovani prirazeni vysledku vyrazu do urcene promenne a uvolneni celeho zasobniku.
 * Nastaveni zasobniku typu do vychoziho stavu.
 *
 * @param variableName - nazev promenne, do ktere bude vysledek prirazen (char*)

 */
void getResult(char *variableName, bool isFunction) {
    if (!isFunction) {
        printf("POPS TF@_exprResult\n");
        printf("MOVE LF@_%s TF@_exprResult\n", variableName);
        printf("POPFRAME\n");
        typeStackDispose(&typeStack);
    } else {
        printf("MOVE TF@_%s TF@_returnValue\n", variableName);
    }
}

//================================================================================
// Generovani instrukci pro vytvoreni a volani fce
//================================================================================

/**
 * Generovani instrukci pro pripravu ramce pred volanim funkce.
 */
void functionFramePreparation() {
    printf("PUSHFRAME\n");
    printf("CREATEFRAME\n");
    //printf("DEFVAR LF@_returnValue\n");
    variableQuantity = 0;
}

/**
 * Generovani instrukce na vytvoreni nove promenne a ulozeni hodnoty do teto promenne.
 *
 * @param t - token (Token)
 */
void callParamLoad(Token t) {
    printf("DEFVAR TF@_%d\n", variableQuantity);
    if (t.type == ID) {
        printf("MOVE TF@_%d LF@_%s\n", variableQuantity, t.val);
    } else if (t.type == INTEGER) {
        printf("MOVE TF@_%d int@%s\n", variableQuantity, t.val);
    } else if (t.type == DOUBLE) {
        printf("MOVE TF@_%d float@%s\n", variableQuantity, t.val);
    } else if (t.type == STRING) {
        printf("MOVE TF@_%d string@%s\n", variableQuantity, t.val);
    }
    variableQuantity++;
}

/**
 * Generovani instrukci pro ulozeni parametu fce na zasobnik.
 */
void callParamsPush() {
    while (variableQuantity > 0) {
        variableQuantity--;
        printf("PUSHS TF@_%d\n", variableQuantity);
    }
}

/**
 * Generovani instrukce na zavolani fce (prechod na navesti)
 *
 * @param functionName - jmeno navesti (char*)
 */
void callInstruction(char *functionName) {
    printf("CALL %s\n", functionName);
    printf("POPFRAME\n");
}

/**
 * Generovani navesti pro funkci
 *
 * @param functionName -jmeno funkce (char*)
 */
void functionDefinition(char *functionName) {
    printf("LABEL %s\n", functionName);
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@_returnValue\n");
}

/**
 * Generovani instrukci pro ziskani hodnoty promenne se zasobniku
 *
 * @param t - token
 */
void functionParamLoad(Token t) {
    printf("DEFVAR TF@_%s\n", t.val);
    printf("POPS TF@_%s\n", t.val);
}

/**
 * Generovani instrukce pro ulozeni navratove hodnoty na zasobnik.
 *
 * @param LFVariable - copy to (char*)
 * @param TFVariable - copy from (char*)
 */
void functionReturn(bool fReturn) {
    if (fReturn == true) {
        printf("POPFRAME\n");
        printf("POPS LF@_returnValue\n");
    }
    printf("RETURN\n");
}

/**
 * Generovani instrukci pro navrat z funkce ber prikazu RETURN.
 *
 * @param type - typ, ktery se bude navrace (TokType)
 */
void functionReturn0(TokType type){
    switch (type){
        case INTEGER:
            printf("MOVE LF@_returnValue int@0\n");
            break;
        case DOUBLE:
            printf("MOVE LF@_returnValue float@0.0\n");
            break;
        case STRING:
            printf("MOVE LF@_returnValue string@!""\n");
            break;
        default:break;
    }
}
//================================================================================
// Vestavene funkce
//================================================================================

/**
 * Generovani vestavene funkce pro zjisteni delky retezce.
 */
void inBuiltLength() {
    printf("LABEL length\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@_returnValue\n");
    printf("DEFVAR TF@_s\n");
    printf("POPS TF@_s\n");
    printf("DEFVAR TF@_Length\n");
    printf("STRLEN TF@_Length TF@_s\n");
    printf("MOVE LF@_returnValue TF@_Length\n");
    printf("RETURN\n");
}

/**
 * Generovani vestavene funkce pro ziskani podretezce od i do n.
 */
void inBuiltSubStr() {
    printf("LABEL substr\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@_returnValue\n");
    printf("MOVE LF@_returnValue string@\n");
    printf("DEFVAR TF@_s\n");
    printf("POPS TF@_s\n");
    printf("DEFVAR TF@_i\n");
    printf("POPS TF@_i\n");
    printf("DEFVAR TF@_n\n");
    printf("POPS TF@_n\n");

    printf("PUSHS TF@_s\n");
    printf("PUSHS string@\n");
    printf("EQS\n");
    printf("PUSHS TF@_i\n");
    printf("PUSHS int@0\n");
    printf("EQS\n");
    printf("PUSHS TF@_i\n");
    printf("PUSHS int@0\n");
    printf("LTS\n");
    printf("ORS\n");
    printf("ORS\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS SubStr$\n");
    printf("RETURN\n");
    printf("LABEL SubStr$\n");
    printf("PUSHS TF@_n\n");
    printf("PUSHS int@0\n");
    printf("LTS\n");
    printf("DEFVAR TF@_length\n");
    functionFramePreparation();
    printf("PUSHS LF@_s\n");
    callInstruction("length");
    printf("MOVE TF@_length TF@_returnValue\n");
    printf("PUSHS TF@_length\n");
    printf("PUSHS TF@_i\n");
    printf("SUBS\n");
    printf("PUSHS TF@_n\n");
    printf("LTS\n");
    printf("ORS\n");
    printf("PUSHS bool@true\n");
    printf("DEFVAR TF@_char\n");
    printf("JUMPIFEQS NOOKSubStr$\n");
    printf("LABEL OKSubStr\n");
    printf("PUSHS TF@_n\n");
    printf("PUSHS int@0\n");
    printf("GTS\n");
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS SubStrEnd$\n");
    printf("GETCHAR TF@_char TF@_s TF@_i\n");
    printf("CONCAT LF@_returnValue LF@_returnValue TF@_char\n");
    printf("SUB TF@_n TF@_n int@1\n");
    printf("ADD TF@_i TF@_i int@1\n");
    printf("JUMP OKSubStr\n");
    printf("LABEL NOOKSubStr$\n");
    printf("PUSHS TF@_length\n");
    printf("PUSHS TF@_i\n");
    printf("GTS\n");
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS SubStrEnd$\n");
    printf("GETCHAR TF@_char TF@_s TF@_i\n");
    printf("CONCAT LF@_returnValue LF@_returnValue TF@_char\n");
    printf("ADD TF@_i TF@_i int@1\n");
    printf("JUMP NOOKSubStr$\n");
    printf("LABEL SubStrEnd$\n");
    printf("RETURN\n");
}

/**
 * Generovani vestavene funkce pro prevedeni hodnoty na znak.
 */
void inBuiltChr() {
    printf("LABEL chr\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@_returnValue\n");
    printf("DEFVAR TF@_i\n");
    printf("POPS TF@_i\n");
    printf("INT2CHAR LF@_returnValue TF@_i\n");
    printf("RETURN\n");
}

/**
 * Generovani vestavene funkce pro ziskani ordinalni hodnoty znaku.
 */
void inBuiltAsc() {
    printf("LABEL asc\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@_returnValue\n");
    printf("DEFVAR TF@_s\n");
    printf("POPS TF@_s\n");
    printf("DEFVAR TF@_i\n");
    printf("POPS TF@_i\n");
    printf("DEFVAR TF@_char\n");
    printf("DEFVAR TF@_length\n");
    functionFramePreparation();
    printf("PUSHS LF@_s\n");
    printf("CALL length\n");
    printf("POPFRAME\n");
    printf("PUSHS TF@_returnValue\n");
    printf("PUSHS TF@_i\n");
    printf("LTS\n");
    printf("PUSHS TF@_returnValue\n");
    printf("PUSHS TF@_i\n");
    printf("EQS\n");
    printf("ORS\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS Asc$\n");
    printf("MOVE LF@_returnValue int@0\n");
    printf("RETURN\n");
    printf("LABEL Asc$\n");
    printf("SUB TF@_i TF@_i int@1\n");
    printf("STRI2INT LF@_returnValue TF@_s TF@_i\n");
    printf("RETURN\n");
}

//================================================================================
// Generovani pomocnych konstrukci
//================================================================================

/**
 * Ulozeni navesti na vrchol zasobniku. A vytvoreni instrukce pro navesti.
 *
 * @param type    - typ konstrukce (labelType)
 * @return status - informace o uspesnosti alokace (int)
 */
int whileIfBegin(labelType type) {
    int status = STATUS_OK;
    if (type == WHILE) {
        status = labelStackPush(&labelStack, type, whileLabelQuantity, false);
        printf("LABEL $$while%d\n", whileLabelQuantity);
        whileLabelQuantity++;
    } else if (type == IF) {
        status = labelStackPush(&labelStack, type, ifLabelQuantity, false);
        printf("LABEL $$if%d\n", ifLabelQuantity);
        ifLabelQuantity++;
    }
    return status;
}

/**
 * Generovani instrukci pro konstrukce IF ,ELSE, WHILE.
 *
 * @param type - typ konstrukce (labelType)
 * @return status - informace o uspesnosti alokace (int)
 */
int whileIfElseEnd(labelType type) {
    labelType lType;
    int status = STATUS_OK;
    int quantity;
    bool ifElse;
    labelStackPop(&labelStack, &lType, &quantity, &ifElse);

    if (type == WHILE) {
        printf("JUMP $$while%d\n", quantity);
        printf("LABEL $$while%dEnd\n", quantity);
    } else if (type == IF) {
        //Pokud nebyl vygenerovano ELSE
        if (!ifElse) {
            printf("LABEL $$else%d\n", quantity);
        }
        printf("LABEL $$if%dEnd\n", quantity);
    } else if (type == ELSE) {
        printf("JUMP $$if%dEnd\n", quantity);
        printf("LABEL $$else%d\n", quantity);
        status = labelStackPush(&labelStack, type, quantity, true);
    }
    return status;
}

/**
 * Generovani prikazu na vytvoreni nove promenne v docasnem ramci.
 *
 * @param name - jmeno promenne (char*)
 */
void variableDeclaration(char *name) {
    printf("DEFVAR TF@_%s\n", name);
}

/**
 * Generovani instrukce pro vypis.
 *
 * @param string      - vypisovany retezec/ jmeno promenne (char *)
 * @param isVariable  - TRUE pokud je retezec promenna (bool)
 */
void write() {
    printf("POPS TF@_exprResult\n");
    printf("WRITE TF@_exprResult\n");
    printf("POPFRAME\n");
}

/**
 * Generovani instrukce pro nacteni do promenne.
 *
 * @param variableName - jmeno promenne (char*)
 * @param type     - typ promenne (TokType)
 */
void input(char *variableName, TokType type) {
    switch (type) {
        case INTEGER:
            printf("READ TF@_%s int\n", variableName);
            break;
        case DOUBLE:
            printf("READ TF@_%s float\n", variableName);
            break;
        case STRING:
            printf("READ TF@_%s string\n", variableName);
            break;
        default:
            break;
    }
}

/**
 * Generovani navesti hlavniho tela programu.
 */
void scopeLabel() {
    printf("LABEL scope\n");
    printf("DEFVAR TF@_returnValue\n");
}
