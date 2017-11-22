#ifndef IFJ_64_2_CODE_GEN_H
#define IFJ_64_2_CODE_GEN_H

#include "scanner.h"
#include "stack_operations.h"

struct typeStackNode *typeStack;
struct labelStackNode *labelStack;

void header();


void typeConvert(bool convert, TokType type);

void convertInstructionSelect(TokType original, TokType new, char *string);

void operationSelect(char operation, bool convert, TokType type);

void boolOperationSelect(char operation, bool convert, TokType type);

bool getOperand(Token *t, bool isVariable, TokType type);

void TFCreation();

void getResult(char *variableName);


bool whileIfBegin(labelType type);

void whileIfElseEnd(labelType type);

void variableDeclaration(char *name);

void write(char *string, bool isVariable);

void input(char *variableName, TokType type);

void scopeLabel();

#endif //TESTBENCH2_CODE_GEN_H
