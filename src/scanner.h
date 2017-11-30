#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define S_END_OF_FILE -1     // returns after succesfully readed file
#define S_TOKEN_OK 0        // returns after token succesfully readed
#define S_LEXEM_FAIL 1      // returns after wrong lexem is readed
#define S_MEMORY_ERROR 99    // returns after memory allocation error

//Martin Stodůlka(xstodu08)
//Ondřej Olšák(xolsak00)
//Michael Schneider(xschne07)
//Marek Kuchynka(xkuchy00)

typedef enum TokenType {INTEGER, DOUBLE, STRING, ID, KEYWORD, DIV, MUL, ADD,
                        SUB, IDIV, EQL, NEQL, LEQL, GEQL, LT, GT, COMMA,
                        SEMICOLON, LBRACKET, RBRACKET, EOL, BOOL
} TokType;


typedef struct token {
  TokType type;
  char* val;
} Token;

Token* FormToken(TokType type, char* value);

int GetToken(Token **token);

void FreeToken(Token **token);


typedef struct GarbageItem {
  Token *token;
  struct GarbageItem *next;
} GarbageItem;

void GarbageFree();


typedef struct buffer {
  char *buffer;
  int used;
  int size;
} DynamicBuffer;

bool BufferInit(DynamicBuffer *b, int size);

bool BufferInsert(DynamicBuffer *b, char c);

#endif // SCANNER_H_INCLUDED
