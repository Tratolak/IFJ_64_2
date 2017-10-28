#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define S_FAILEDOPEN 1

FILE* SOURCE;

typedef enum TokenType {INT, DOUBLE, STRING, ID, KEYWORD, DIV, EOL //TBD
} TokType;

typedef struct token {
  TokType type;
  char* val;
} Token;

enum enState {START, IDENTIFIER, COMLINE, COMBLOCK //TBD
} state;

int SOpenFile(char* path);

int GetToken(Token **token);

void SCloseFile();

#endif // SCANNER_H_INCLUDED
