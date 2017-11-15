#include "stack_operations.h"

/**
 * Inicializace prazdneho zasobniku
 *
 * @param head - neinicializovany zasobnik (struct stackNode**)
 */
void stackInit(struct stackNode **head) {
    *head = NULL;
}

/**
 * Zjisteni zda-li je zasobnik prazdny.
 *
 * @param head - ukazatel na vrchol zasobnik (struct stackNode*)
 * @return true | false
 */
bool stackEmpty(struct stackNode *head) {
    return head == NULL ? true : false;
}

/**
 * Vlozeni typu promenne (TokType) na zasobnik.
 *
 * @param head   - ukazatel na vrchol zasobniku (struct stackNode**)
 * @param type   - vkladany typ hodnoty (TokType)
 * @return false - alokace neprobehla uspesne | true - vse ok
 */
bool stackPush(struct stackNode **head, TokType type) {
    struct stackNode *newNode = (struct stackNode *) malloc(sizeof(struct stackNode));
    if (newNode == NULL) {
        return false;
    }
    (*newNode).type = type;
    (*newNode).next = *head;
    *head = newNode;
    return true;
}

/**
 * Odstraneni TokType z vrcholu zasobniku. U ulozeni hodnoty do promenne 'type'.
 *
 * @param head - ukazatel na vrchol zasobniku (struct stackNode**)
 * @param type - ukazatel na typ promenne (TokType)
 */
void stackPop(struct stackNode **head, TokType *type) {
    struct stackNode *node = *head;
    *type = (*head)->type;
    *head = (*head)->next;
    free(node);
}