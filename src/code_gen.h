#ifndef TESTBENCH2_CODE_GEN_H
#define TESTBENCH2_CODE_GEN_H

#include "scanner.h"

struct stackNode* typeStack;

void header();

void typeConvert(bool convert, TokType type);

void convertInstructionSelect(TokType original, TokType new, char *string);

void actionSelect(char operation,bool convert,TokType type);

void value(Token *t);

void TFCreation();

void getResult();

#endif //TESTBENCH2_CODE_GEN_H
