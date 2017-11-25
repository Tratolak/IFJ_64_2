#ifndef SYNTAX_ANALYZER_H_INCLUDED
#define SYNTAX_ANALYZER_H_INCLUDED

#include "scanner.h"

//Martin Stodùlka(xstodu08)
//Ondøej Olšák(xolsak00)
//Michael Schneider(xschne07)
//Marek Kuchynka(xkuchy00)

int SyntaxAnalyzer();
int PreAnalyzer(Token *act,Token **back, TokType * res, int fce);
int PrePosition(char c);
#endif // SYNTAX_ANALYZER_H_INCLUDED
