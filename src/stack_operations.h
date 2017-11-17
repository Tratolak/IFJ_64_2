#ifndef IFJ_64_2_STACK_OPERATIONS_H
#define IFJ_64_2_STACK_OPERATIONS_H


#define MAX_STACK 1

/**
 * Definice struktury zasobniku
 */
typedef struct {
    char arr[MAX_STACK];                             /* pole pro uložení hodnot */
    int top;                                /* index prvku na vrcholu zásobníku */
} tStack;

void stackInit(tStack* s);

int stackEmpty(const tStack* s);

int stackFull(const tStack* s);

void stackTop(const tStack* s, char* c);

void stackPop(tStack* s);

void stackPush(tStack* s, char c);


#endif //IFJ_64_2_STACK_OPERATIONS_H
