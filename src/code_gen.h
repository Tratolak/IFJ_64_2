#ifndef IFJ_64_2_CODE_GEN_H
#define IFJ_64_2_CODE_GEN_HT

#include "scanner.h"
#include "stack_operations.h"

struct typeStackNode *typeStack;
struct labelStackNode *labelStack;

void header();


void typeConvert(bool convert, TokType type);

void convertInstructionSelect(TokType original, TokType new, char *string);

void operationSelect(char operation, bool convert, TokType type);

void boolOperationSelect(char operation, TokType var1, TokType var2);

void boolOperationConvert(TokType var1, TokType var2);

bool getOperand(Token *t);

void TFCreation();

void getResult(char *variableName, bool isFunction);

void functionFramePreparation();

void callParamLoad(Token t);

void callParamsPush();

void callInstruction(char *functionName);

void functionDefinition(char *functionName);

void functionParamLoad(Token t);

void functionReturn(bool fReturn);

bool whileIfBegin(labelType type);

void whileIfElseEnd(labelType type);

void variableDeclaration(char *name);

void write();

void input(char *variableName, TokType type);

void scopeLabel();

#endif //TESTBENCH2_CODE_GEN_H
