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
 * @param convert   - true = je treba pretypovat | false= neni treba pretypovat (bool)
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
 * Ulozeni hodnoty/promenne na vrchol zasobniku.
 *
 * @param t - promenna/absolutni hodnota (Token)
 * @return mallocOk - true = ok | false = error (bool)
 */
bool getOperand(Token *t) {
    bool mallocOk = true;
    if (t->type == INTEGER) {
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
 * Generovani instrukce pro ulozeni navratove hodnoty z LF do TF.
 *
 * @param LFVariable - copy to (char*)
 * @param TFVariable - copy from (char*)
 */
void functionEnd(char *LFVariable, char *TFVariable) {
    printf("MOVE LF@_%s TF@_%s\n", LFVariable, TFVariable);
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
 * Ulozeni navesti na vrchol zasobniku.
 *       //TODO
 * @return mallocOk - true = ok | false = error (bool)
 */
bool whileIfBegin(labelType type) {
    bool mallocOk = true;
    if (type == WHILE) {
        mallocOk = labelStackPush(&labelStack,"While", type, whileLabelQuantity);
        whileLabelQuantity++;
        printf("LABEL while%d\n",whileLabelQuantity);
        //Generovani podminky
    } else if (type == IF) {
        mallocOk = labelStackPush(&labelStack, "If", type, ifLabelQuantity);
        ifLabelQuantity++;
    }
    return mallocOk;
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
 * @param variable - jmeno promenne (char*)
 * @param type     - typ promenne (TokType)
 */
void input(char *variable, TokType type) {
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
    printf("READ TF@_%s %s\n", variable, inputType);
}