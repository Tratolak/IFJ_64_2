#include <stdio.h>
#include "code_gen.h"
#include "stack_operations.h"


/**
 * Utvoreni povinne hlavicky IFJcode17
 */
void header() {
    printf(".IFJcode17\n");
}

/**
 * Vytvoreni docasneho ramce pro vypocet vyrazu. A inicializace zasobniku typu.
 */
void TFCreation() {
    stackInit(&typeStack);
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
    stackPop(&typeStack, &first);
    stackPop(&typeStack, &second);

    if (convert) {
        if (first != type)
            convertInstructionSelect(first, type, "TF@_exprOperand1");
        if (second != type)
            convertInstructionSelect(second, type, "TF@_exprOperand2");
    }
    stackPush(&typeStack, type);
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
 * @param operation - provadena operace
 */
void actionSelect(char operand, bool convert, TokType type) {
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
        default://je to hodnota
            //TODO something is wrong :D
            break;
    }

}

/**
 * Ulozeni hodnoty/promenne na vrchol zasobniku.
 *
 * @param t - promenna/absolutni hodnota
 */
void value(Token *t) {
    if (t->type == INTEGER) {
        printf("PUSHS int@%s\n", t->val);
        stackPush(&typeStack, t->type);
    } else if (t->type == DOUBLE) {
        printf("PUSHS float@%s\n", t->val);
        stackPush(&typeStack, t->type);
    } else if (t->type == STRING) {
        printf("PUSHS string@%s\n", t->val);
        stackPush(&typeStack, t->type);
    } else if (t->type == ID) {
        printf("PUSHS %s\n", t->val);
        stackPush(&typeStack, t->type);
    }
}

/**
 * //TODO vlozeni vysledku do promene
 */
void getResult() {
    printf("POPS TF@_exprResult\n");
    printf("WRITE TF@_exprResult\n");
}