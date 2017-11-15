#ifndef TESTBENCH2_STACK_OPERATIONS_H
#define TESTBENCH2_STACK_OPERATIONS_H

#include "scanner.h"

struct stackNode {
    TokType type;
    struct stackNode *next;
};

void stackInit(struct stackNode **head);

bool stackEmpty(struct stackNode *head);

bool stackPush(struct stackNode **head, TokType type);

void stackPop(struct stackNode **head, TokType *type);

#endif //TESTBENCH2_STACK_OPERATIONS_H
