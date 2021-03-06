#ifndef IFJ_64_2_CODE_GEN_H
#define IFJ_64_2_CODE_GEN_H

//Martin Stodůlka(xstodu08)
//Ondřej Olšák(xolsak00)
//Michael Schneider(xschne07)
//Marek Kuchynka(xkuchy00)

#include "scanner.h"
#include "stack_operations.h"

#define STATUS_OK  0;
#define STATUS_MALLOC_ERR  1;

struct typeStackNode *typeStack;
struct labelStackNode *labelStack;

void header();

int typeConvert(char operand, bool convert, TokType type);

void convertInstructionSelect(TokType original, TokType new, char *string);

int operationSelect(char operation, bool convert, TokType type);

void boolOperationSelect(char operation, TokType var1, TokType var2);

void boolOperationConvert(TokType var1, TokType var2);

int getOperand(Token *t);

void TFCreation();

void retype(TokType original, TokType new);

void getResult(char *variableName, bool isFunction);

void functionFramePreparation();

void callParamLoad(Token t);

void callParamsPush();

void callInstruction(char *functionName);

void functionDefinition(char *functionName);

void functionParamLoad(Token t);

void functionReturn(bool fReturn);

void functionReturn0(TokType type);

void inBuiltLength();

void inBuiltSubStr();

void inBuiltChr();

void inBuiltAsc();

int whileIfBegin(labelType type);

int whileIfElseEnd(labelType type);

void variableDeclaration(char *name, TokType type);

void write();

void input(char *variableName, TokType type);

void scopeLabel();

#endif //TESTBENCH2_CODE_GEN_H
