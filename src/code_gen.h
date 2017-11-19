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

bool getOperand(Token *t);

void TFCreation();

void getResult(char *variableName);


void functionBegin(char *functionName);

void functionEnd(char *LFVariable, char *TFVariable);

void functionCall(char *functionName);

bool whileIfBegin(labelType type);

void variableDeclaration(char *name);


void input(char *variable, TokType type);

#endif //TESTBENCH2_CODE_GEN_H
