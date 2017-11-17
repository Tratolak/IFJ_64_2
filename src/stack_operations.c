#include "stack_operations.h"

/**
 * Inicializace prazdneho zasobniku pto ukladani typu promennych.
 *
 * @param head - neinicializovany zasobnik (struct typeStackNode**)
 */
void typeStackInit(struct typeStackNode **head) {
    *head = NULL;
}

/**
 * Inicializace prazdneho zasobniku pro ukladani navesti.
 *
 * @param head - neinicializovany zasobnik (struct labelStackNode**)
 */
void labelStackInit(struct labelStackNode **head) {
    *head = NULL;
}

/**
 * Zjisteni zda-li je zasobnik prazdny.
 *
 * @param head - ukazatel na vrchol zasobnik (struct typeStackNode*)
 * @return true | false (bool)
 */
bool typeStackEmpty(struct typeStackNode *head) {
    return head == NULL ? true : false;
}

/**
 * Zjisteni zda-li je zasobnik prazdny.
 *
 * @param head - ukazatel na vrchol zasobnik (struct labelStackNode*)
 * @return true | false (bool)
 */
bool labelStackEmpty(struct labelStackNode *head) {
    return head == NULL ? true : false;
}

/**
 * Vlozeni typu promenne (TokType) na zasobnik.
 *
 * @param head   - ukazatel na vrchol zasobniku (struct typeStackNode**)
 * @param type   - vkladany typ hodnoty (TokType)
 * @return false - false = alokace neprobehla uspesne | true - vse ok
 */
bool typeStackPush(struct typeStackNode **head, TokType type) {
    struct typeStackNode *newNode = (struct typeStackNode *) malloc(sizeof(struct typeStackNode));
    if (newNode == NULL) {
        return false;
    }
    (*newNode).type = type;
    (*newNode).next = *head;
    *head = newNode;
    return true;
}

/**
 * Odstraneni TokType z vrcholu zasobniku. Ulozeni typu do promenne 'type'.
 *
 * @param head - ukazatel na vrchol zasobniku (struct typeStackNode**)
 * @param type - ukazatel na typ promenne (TokType)
 */
void typeStackPop(struct typeStackNode **head, TokType *type) {
    struct typeStackNode *node = *head;
    *type = (*head)->type;
    *head = (*head)->next;
    free(node);
}

/**
 * Vlozeni typu a jmena generovanych navesti na zasobnik.
 *
 * @param head      - ukazatel na vrchol zasobniku (struct labelStackNode**)
 * @param labelName - jmeno navesti (char *)
 * @param labelType - typ navesti (labelType)
 * @return false    - false  = alokace neprobehla uspesne | true = vse ok
 */
bool labelStackPush(struct labelStackNode **head,char *labelName,labelType labelType) {
    struct labelStackNode *newNode = (struct labelStackNode *) malloc(sizeof(struct labelStackNode));
    if (newNode == NULL) {
        return false;
    }
    (*newNode).labelName = labelName;
    (*newNode).labelType = labelType;
    (*newNode).next = *head;
    *head = newNode;
    return true;
}

/**
 * Odstraneni navesti z vrcholu zasobniku. Ulozi jmeno navesi do promenne 'labelName' a typ navesti do labelType.
 *
 * @param head      - ukazatel na vrchol zasobniku (struct labelStackNode**)
 * @param labelName - jmeno navesti (char *)
 * @param labelType - typ navesti (labelType)
 */
void labelStackPop(struct labelStackNode **head,char **labelName,labelType *labelType) {
    struct labelStackNode *node = *head;
    *labelType = (*head)->labelType;
    *labelName = (*head)->labelName;
    *head = (*head)->next;
    free(node);
}