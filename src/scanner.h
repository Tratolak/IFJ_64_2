#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <ctype.h>
#include <stdio.h>
#include "syntax_analyzer.h"

#define S_FAILEDOPEN 1

FILE* SOURCE;

int SOpenFile(char* path);
Token GetToken();
void SCloseFile();

#endif // SCANNER_H_INCLUDED
