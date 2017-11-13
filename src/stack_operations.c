#include <stdio.h>
#include "stack_operations.h"


int STACK_SIZE = MAX_STACK;

void stackInit(tStack *s) {
    if (s == NULL) {
        printf("stackerror");
    } else {
        s->arr[0] = '$';
        s->top = 0;          //inicializace vrcholu + zarazka pro precedencni analyzu
    }
}

int stackEmpty(const tStack *s) {
    return s->top == -1 ? 1 : 0;
}

int stackFull(const tStack *s) {
    return s->top == STACK_SIZE - 1 ? 1 : 0;
}

void stackTop(const tStack *s, char *c) {
    if (stackEmpty(s)) { //abychom nesahali nahodne do pameti
        printf("stackerror");
    } else {
        *c = s->arr[s->top];
    }
}


void stackPop(tStack *s) {
    if (stackEmpty(s)) {
        //tady by bylo to varovaní
    } else {
        s->top--;
    }
}


void stackPush(tStack *s, char c) {
    if (stackFull(s)) {
        printf("stackerror");
    } else {
        s->top++;                        //posunu a vlozim
        s->arr[s->top] = c;
    }
}