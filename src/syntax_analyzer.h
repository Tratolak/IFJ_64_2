#ifndef SYNTAX_ANALYZER_H_INCLUDED
#define SYNTAX_ANALYZER_H_INCLUDED

#include "scanner.h"

//TBD

typedef struct DeTree
{
    Token* token;
    struct BT* LPtr;
    struct BT* RPtr;
} DT;

DT* SynAnalyzer(DT* node);

#endif // SYNTAX_ANALYZER_H_INCLUDED
