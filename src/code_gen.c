#include <stdio.h>
#include "code_gen.h"
#include "stack_operations.h"

//Pocitani poctu navesti daneho typu
int whileLabelQuantity = 0;
int ifLabelQuantity = 0;

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
    printf("JMP scope\n");
}

//================================================================================
// Funkce potrebne pro generovani instrukci na vyhodnoceni vyrazu
//================================================================================

/**
/**
 * Vytvoreni docasneho ramce pro vypocet vyrazu. A inicializace zasobniku typu.
 */
void TFCreation() {
    typeStackInit(&typeStack);
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
 */
void typeConvert(bool convert, TokType type) {
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
    typeStackPush(&typeStack, type);
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
void operationSelect(char operand, bool convert, TokType type) {
    typeConvert(convert, type);
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
}

/**
 * Generovani vyrazu pro konstrukce IF,IF ELSE a WHILE.
 *
 * @param operation - provadena operace (char)
 * @param convert   - true = je treba pretypovat | false = neni treba pretypovat (bool)
 * @param type      - typ do ktereho se provede pripadne pretypovani (TokType)
 */
void boolOperationSelect(char operand, bool convert, TokType type) {
    typeConvert(convert, type);
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
                printf("JUMPIFNEQS while%dEnd\n", quantity); //while%dEnd - navesti konce cyklu
            } else if (lType == IF) {
                printf("JUMPIFNEQS else%d\n", quantity); //if%dFalse - zacatek false vetve podminky
            }
            break;
        case '#':  //!=
            printf("PUSHS TF@_exprOperand2\n");
            printf("PUSHS TF@_exprOperand1\n");
            printf("EQS\n");
            printf("PUSHS bool@false\n");
            labelStackTop(&labelStack, &lType, &quantity);
            if (lType == WHILE) {
                printf("JUMPIFNEQS while%dEnd\n", quantity); //while%dEnd - navesti konce cyklu
            } else if (lType == IF) {
                printf("JUMPIFNEQS else%d\n", quantity); //if%dFalse - zacatek false vetve podminky
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
                printf("JUMPIFNEQS while%dEnd\n", quantity); //while%dEnd - navesti konce cyklu
            } else if (lType == IF) {
                printf("JUMPIFNEQS else%d\n", quantity); //if%dFalse - zacatek false vetve podminky
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
                printf("JUMPIFNEQS while%dEnd\n", quantity); //while%dEnd - navesti konce cyklu
            } else if (lType == IF) {
                printf("JUMPIFNEQS else%d\n", quantity); //if%dFalse - zacatek false vetve podminky
            }
            break;
        case '?': // <
            printf("PUSHS TF@_exprOperand2\n");
            printf("PUSHS TF@_exprOperand1\n");
            printf("LTS\n");
            printf("PUSHS bool@true\n");
            labelStackTop(&labelStack, &lType, &quantity);
            if (lType == WHILE) {
                printf("JUMPIFNEQS while%dEnd\n", quantity); //while%dEnd - navesti konce cyklu
            } else if (lType == IF) {
                printf("JUMPIFNEQS else%d\n", quantity); //if%dFalse - zacatek false vetve podminky
            }
            break;
        case ':': //>
            printf("PUSHS TF@_exprOperand2\n");
            printf("PUSHS TF@_exprOperand1\n");
            printf("GTS\n");
            printf("PUSHS bool@true\n");
            labelStackTop(&labelStack, &lType, &quantity);
            if (lType == WHILE) {
                printf("JUMPIFNEQS while%dEnd\n", quantity); //while%dEnd - navesti konce cyklu
            } else if (lType == IF) {
                printf("JUMPIFNEQS else%d\n", quantity); //if%dFalse - zacatek false vetve podminky
            }
        default:
            break;
    }
    printf("CLEARS\n");
}

/**
 * Ulozeni hodnoty/promenne na vrchol zasobniku.
 *
 * @param t          - promenna/absolutni hodnota (Token)
 * @param isVariable - true jedna-li se o promennou (bool)
 * @param type       - typ promenne (TokType)
 * @return mallocOk - true = ok | false = error (bool)
 */
bool getOperand(Token *t, bool isVariable, TokType type) {
    bool mallocOk = true;
    if (isVariable) {
        //Timto si nejsem uplne jisty. Sem se program dostane, kdyz je ve vyrazu promenna.
        printf("PUSHS TF@_%s\n", t->val);
        mallocOk = typeStackPush(&typeStack, type);
    } else if (t->type == INTEGER) {
        printf("PUSHS int@%s\n", t->val);
        mallocOk = typeStackPush(&typeStack, t->type);
    } else if (t->type == DOUBLE) {
        printf("PUSHS float@%s\n", t->val);
        mallocOk = typeStackPush(&typeStack, t->type);
    } else if (t->type == STRING) {
        printf("PUSHS string@%s\n", t->val);
        mallocOk = typeStackPush(&typeStack, t->type);
    } else if (t->type == ID) {
        printf("PUSHS %s\n", t->val);
        mallocOk = typeStackPush(&typeStack, t->type);
    }
    return mallocOk;
}


/**
 * Generovani prirazeni vysledku vyrazu do urcene promenne a uvolneni celeho zasobniku.
 * Nastaveni zasobniku typu do vychoziho stavu.
 *
 * @param variableName - nazev promenne, do ktere bude vysledek prirazen (char*)
 */
void getResult(char *variableName) {
    printf("POPS TF@_exprResult\n");
    printf("MOVE TF@_%s TF@_expResult\n", variableName);
    printf("CLEARS\n");
    typeStackDispose(&typeStack);
}

//================================================================================
// Generovani instrukci pro vytvoreni a volani fce
//================================================================================

/**
 * Generovani instrukci navesti funkce, ulozeni TF na zasobnik a utvoreni noveho TF.
 *
 * @param functionName - jmeno funkce (char*)
 */
void functionBegin(char *functionName) {
    printf("LABEL %s\n", functionName);
    printf("PUSHFRAME\n");
    printf("CREATEFRAME\n");
}

/**
 * Generovani instrukce pro ulozeni navratove hodnoty z LF do TF. Pokud fce nic nevraci jsou oba
 * predane parametry nastaveny na "".
 *
 * @param LFVariable - copy to (char*)
 * @param TFVariable - copy from (char*)
 */
void functionEnd(char *LFVariable, char *TFVariable) {
    if (LFVariable != "" && TFVariable != "") {
        printf("MOVE LF@_%s TF@_%s\n", LFVariable, TFVariable);
    }
    printf("RETURN\n");
}

/**
 * Generovani instrukci pro volani fce a odstraneni TF utvoreneho ve fci.
 *
 * @param functionName - jmeno navesti zacatku funkce (char*)
 */
void functionCall(char *functionName) {
    printf("CALL %s\n", functionName);
    printf("POPFRAME\n");
}

//================================================================================
// Generovani pomocnych konstrukci
//================================================================================

/**
 * Ulozeni navesti na vrchol zasobniku. A vytvoreni instrukce pro navesti.
 *
 * @param type - typ konstrukce (labelType)
 * @return mallocOk - true = ok | false = error (bool)
 */
bool whileIfBegin(labelType type) {
    bool mallocOk = true;
    if (type == WHILE) {
        mallocOk = labelStackPush(&labelStack, type, whileLabelQuantity, false);
        printf("LABEL while%d\n", whileLabelQuantity);
        whileLabelQuantity++;
    } else if (type == IF) {
        mallocOk = labelStackPush(&labelStack, type, ifLabelQuantity, false);
        printf("LABEL if%d\n", ifLabelQuantity);
        ifLabelQuantity++;
    }
    return mallocOk;
}

/**
 * Generovani instrukci pro konstrukce IF ,ELSE, WHILE.
 *
 * @param type - typ konstrukce (labelType)
 */
void whileIfElseEnd(labelType type) {
    labelType lType;
    int quantity;
    bool ifElse;
    labelStackPop(&labelStack, &lType, &quantity, &ifElse);

    if (type == WHILE) {
        printf("JUMP while%d\n", quantity);
        printf("LABEL while%dEnd\n", quantity);
    } else if (type == IF) {
        //Pokud nebyl vygenerovano ELSE
        if (!ifElse) {
            printf("LABEL else%d\n", quantity);
        }
        printf("LABEL if%dEnd\n", quantity);
    } else if (type == ELSE) {
        printf("JUMP if%dEnd\n", quantity);
        printf("LABEL else%d\n", quantity);
        labelStackPush(&labelStack, type, ifLabelQuantity, true);
    }
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
 * Generovani instrukce pro nacteni do promenne.
 *
 * @param variableName - jmeno promenne (char*)
 * @param type     - typ promenne (TokType)
 */
void input(char *variableName, TokType type) {
    char *inputType = "";
    switch (type) {
        case INTEGER:
            inputType = "int";
            break;
        case DOUBLE:
            inputType = "float";
            break;
        case STRING:
            inputType = "string";
            break;
        default:
            break;
    }
    printf("READ TF@_%s %s\n", variableName, inputType);
}