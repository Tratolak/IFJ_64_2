#include <stdio.h>
#include "code_gen.h"
#include "stack_operations.h"

//Pocitani poctu navesti daneho typu
int whileLabel = 0;
int ifLabel = 0;

/**
 * Utvoreni povinne hlavicky IFJcode17, inicializace zasobniku pro navesti.
 * Instrukce pro skok na navesti zacatku programu.
 */
void header() {
    printf(".IFJcode17\n");
    labelStackInit(&labelStack);
    printf("JMP scope\n");
}

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
            if(type != DOUBLE){
                convertInstructionSelect(type, DOUBLE, "TF@_exprResult");
            }
            printf("PUSHS TF@_exprResult\n");
                break;
        case '\\': //Operandy a vysledek jsou typu INTEGER
                printf("DIV TF@_exprResult TF@_exprOperand2 TF@_exprOperand1\n");
                printf("PUSHS TF@_exprResult\n");
            break;
        default:break;
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
 * Ulozeni navesti na vrchol zasobniku.
 *
 * @return mallocOk - true = ok | false = error (bool)
 */
bool getLabel(char *label){
    bool mallocOk = true;

    return mallocOk;
}

/**
 * //TODO vlozeni vysledku do promene
 */
void getResult() {
    printf("POPS TF@_exprResult\n");
    printf("WRITE TF@_exprResult\n");
}