#ifndef IFJ_64_2_CODE_GEN_H
#define IFJ_64_2_CODE_GEN_H

#include "scanner.h"

struct typeStackNode* typeStack;
struct labelStackNode* labelStack;

void header();

void typeConvert(bool convert, TokType type);

void convertInstructionSelect(TokType original, TokType new, char *string);

void operationSelect(char operation,bool convert,TokType type);

bool getOperand(Token *t);

bool getLabel(char* label);

void TFCreation();

void getResult();

#endif //TESTBENCH2_CODE_GEN_H
