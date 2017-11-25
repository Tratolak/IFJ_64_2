#ifndef IFJ_64_2_STACK_OPERATIONS_H
#define IFJ_64_2_STACK_OPERATIONS_H

#include "scanner.h"
#define STATUS_OK  0;
#define STATUS_MALLOC_ERR  1;

//Martin Stodùlka(xstodu08)
//Ondøej Olšák(xolsak00)
//Michael Schneider(xschne07)
//Marek Kuchynka(xkuchy00)

typedef enum enLabelType {
    WHILE, IF, ELSE
} labelType;

struct typeStackNode {
    TokType type;
    struct typeStackNode *next;
};

struct labelStackNode {
    labelType labelType;
    bool ifElse;
    int quantity;
    struct labelStackNode *next;
};

void typeStackInit(struct typeStackNode **head);


int typeStackPush(struct typeStackNode **head, TokType type);

void typeStackPop(struct typeStackNode **head, TokType *type);

void typeStackDispose(struct typeStackNode **head);


void labelStackInit(struct labelStackNode **head);


int labelStackPush(struct labelStackNode **head, labelType labelType, int quantity, bool ifElse);

void labelStackPop(struct labelStackNode **head, labelType *labelType, int *quantity, bool *ifElse);

void labelStackTop(struct labelStackNode **head, labelType *labelType, int *quantity);

void labelStackDispose(struct labelStackNode **head);

#endif //TESTBENCH2_STACK_OPERATIONS_H
