#include "stack_operations.h"

//================================================================================
// Funkce pro praci se zasobnikem typu promennych
//================================================================================

/**
 * Inicializace prazdneho zasobniku pto ukladani typu promennych.
 *
 * @param head - neinicializovany zasobnik (struct typeStackNode**)
 */
void typeStackInit(struct typeStackNode **head) {
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
 * Zruseni vsech uzlu zasobniku. Nastaveni zasobniku na pocatecni hodnotu.
 *
 * @param head - ukazatel na vrchol zasobniku (struct typeStackNode**)
 */
void typeStackDispose(struct typeStackNode **head) {
    struct typeStackNode *node;
    while (*head != NULL) {
        node = *head;
        *head = (*head)->next;
        free(node);
    }
}

//================================================================================
// Funkce pro praci se zasobnikem navesti
//================================================================================

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
 * @param head - ukazatel na vrchol zasobnik (struct labelStackNode*)
 * @return true | false (bool)
 */
bool labelStackEmpty(struct labelStackNode *head) {
    return head == NULL ? true : false;
}

/**
 * Vlozeni typu a jmena generovanych navesti na zasobnik.
 *
 * @param head      - ukazatel na vrchol zasobniku (struct labelStackNode**)
 * @param labelName - jmeno navesti (char *)
 * @param labelType - typ navesti (labelType)
 * @param ifElse    - plati pouze pro if a indikuje yda-li byl jiz vygenerovan ELSE
 * @return false    - false  = alokace neprobehla uspesne | true = vse ok
 */
bool labelStackPush(struct labelStackNode **head, labelType labelType, int quantity, bool ifElse) {
    struct labelStackNode *newNode = (struct labelStackNode *) malloc(sizeof(struct labelStackNode));
    if (newNode == NULL) {
        return false;
    }
    (*newNode).labelType = labelType;
    (*newNode).quantity = quantity;
    (*newNode).ifElse = ifElse;
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
 * @param ifElse    - plati pouze pro if a indikuje yda-li byl jiz vygenerovan ELSE
 * @param quantity  - pocitadlo: o kolikate navesti daneho typu se jedna (int*)
 */
void labelStackPop(struct labelStackNode **head, labelType *labelType, int *quantity, bool *ifElse) {
    struct labelStackNode *node = *head;
    *labelType = (*head)->labelType;
    *quantity = (*head)->quantity;
    *ifElse = (*head)->ifElse;
    *head = (*head)->next;
    free(node);
}

/**
 * Precte navesti z vrcholu zasobniku. Ulozi jmeno navesi do promenne 'labelName' a typ navesti do labelType.
 *
 * @param head      - ukazatel na vrchol zasobniku (struct labelStackNode**)
 * @param labelName - jmeno navesti (char *)
 * @param labelType - typ navesti (labelType)
 * @param quantity  - pocitadlo: o kolikate navesti daneho typu se jedna (int*)
 */
void labelStackTop(struct labelStackNode **head, labelType *labelType, int *quantity) {
    *labelType = (*head)->labelType;
    *quantity = (*head)->quantity;
}

/**
 * Zruseni vsech uzlu zasobniku. Nastaveni zasobniku na pocatecni hodnotu.
 *
 * @param head - ukazatel na vrchol zasobniku (struct labelStackNode**)
 */
void labelStackDispose(struct labelStackNode **head) {
    struct labelStackNode *node;
    while (*head != NULL) {
        node = *head;
        *head = (*head)->next;
        free(node);
    }
}