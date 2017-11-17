#ifndef IFJ_64_2_STACK_OPERATIONS_H
#define IFJ_64_2_STACK_OPERATIONS_H

#include "scanner.h"

typedef enum enLabelType{
    WHILE, IF
}labelType;

struct typeStackNode {
    TokType type;
    struct typeStackNode *next;
};

struct labelStackNode {
    labelType labelType;
    char *labelName;
    struct labelStackNode *next;
};

void typeStackInit(struct typeStackNode **head);

void labelStackInit(struct labelStackNode **head);

bool typeStackEmpty(struct typeStackNode *head);

bool labelStackEmpty(struct labelStackNode *head);

bool typeStackPush(struct typeStackNode **head, TokType type);

void typeStackPop(struct typeStackNode **head, TokType *type);

bool labelStackPush(struct labelStackNode **head, char *labelName, labelType labelType);

void labelStackPop(struct labelStackNode **head, char **labelName, labelType *labelType);

#endif //TESTBENCH2_STACK_OPERATIONS_H
