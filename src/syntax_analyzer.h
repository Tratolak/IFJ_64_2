#ifndef SYNTAX_ANALYZER_H_INCLUDED
#define SYNTAX_ANALYZER_H_INCLUDED

#include "scanner.h"

//TBD

int SyntaxAnalyzer();
int PreAnalyzer(Token *act,Token **back);
int PrePosition(char c);
#endif // SYNTAX_ANALYZER_H_INCLUDED
