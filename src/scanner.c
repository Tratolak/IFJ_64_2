#include "scanner.h"

// reserved words from the language  (35)
const char * const Reserved[] = {"as", "asc", "declare", "dim", "do", "double",
"else", "end", "chr", "function", "if", "input", "integer", "length", "loop",
"print", "return", "scope", "string", "substr", "then", "while", "and",
"boolean", "continue", "elseif", "exit", "false", "for", "next", "not", "or",
"shared", "static", "true"};

Token* FormToken(TokType type, char* value) {
  Token *token = malloc(sizeof(Token));
  if (token == NULL)
    return NULL;

  token->type = type;
  token->val = value;

  return token;
}

int IsReserved(char* str) {
  for (int i = 0; i < 35; i++) {
    if (strcmp(str, Reserved[i]) == 0)
      return true;
  }
  return false;
}

bool BufferInit(DynamicBuffer *b, int size) {
  b->buffer = (char *) malloc(size * sizeof(char));
  if (b->buffer == NULL)
    return false;
  b->used = 0;
  b->size = size;
  return true;
}

void BufferInsert(DynamicBuffer *b, char c) {
  if (b->used == b->size) {
    b->size += 32;
    b->buffer = (char *)realloc(b->buffer, b->size + sizeof(char) * 32);
  }
  b->buffer[b->used++] = c;
}

int GetToken(Token **token) {
  int c;
  bool t = false;
  DynamicBuffer str;
  state = START;

  while ((c = getchar()) != EOF) {

    if (c == '\r')
      continue;

    c = tolower(c);

    switch (state) {

      case START:

        // skip white & 0
        if (isblank(c) || c == '0')
          break;

        // new line
        else if (c == '\n') {
          *token = FormToken(EOL, NULL);
          t = true;
        }

        // identifier/keyword
        else if (islower(c) || c == '_') {
          state = IDENTIFIER;
          if (!BufferInit(&str, 32))
            return S_MEMORY_ERROR;
          BufferInsert(&str, c);
        }

        else if (isdigit(c)) {
          state = INT;
          if (!BufferInit(&str, 32))
            return S_MEMORY_ERROR;
          BufferInsert(&str, c);
        }

        // line comment
        else if (c == '\'')
          state = COMLINE;

        // block comment or dividing
        else if (c == '/')
          state = SLASH;

        // multiplication
        else if (c == '*') {
          *token = FormToken(MUL, NULL);
          t = true;
        }

        // adding
        else if (c == '+') {
          *token = FormToken(ADD, NULL);
          t = true;
        }

        // substract
        else if (c == '-') {
          *token = FormToken(SUB, NULL);
          t = true;
        }

        // modulo
        else if (c == '\\') {
          *token = FormToken(MOD, NULL);
          t = true;
        }

        // equal
        else if (c == '=') {
          *token = FormToken(EQL, NULL);
          t = true;
        }

        // less-than (and something)
        else if (c == '<')
          state = LESS;

        // greater-than (and something)
        else if (c == '>')
          state = GREATER;

        // unexpected character
        else {
          fprintf(stderr, "ERROR: unexpected symbol \"%c\".\n", c);
          return S_LEXEM_FAIL;
        }

        break;


      case IDENTIFIER:
        if (isalnum(c) || c == '_')
          BufferInsert(&str, c);
        else {
          BufferInsert(&str, '\0');
          ungetc(c, stdin);
          if (IsReserved(str.buffer))
            *token = FormToken(KEYWORD, str.buffer);
          else
            *token = FormToken(ID, str.buffer);
          t = true;
        }
        break;


      case INT:
        if (isdigit(c))
          BufferInsert(&str, c);
        else if (c == '.') {
          state = DOUBLE_N;
          BufferInsert(&str, c);
        }
        else if (c == 'e') {
          state = DOUBLE_ES;
          BufferInsert(&str, c);
        }
        else {
          BufferInsert(&str, '\0');
          ungetc(c, stdin);
          *token = FormToken(INTEGER, str.buffer);
          t = true;
        }
        break;


      case DOUBLE_N:
        if (isdigit(c))
          BufferInsert(&str, c);
        else if (c == 'e') {
          state = DOUBLE_ES;
          BufferInsert(&str, c);
        }
        else {
          BufferInsert(&str, '\0');
          ungetc(c, stdin);
          *token = FormToken(DOUBLE, str.buffer);
          t = true;
        }
        break;


      case DOUBLE_ES:
        if (isdigit(c)) {
          state = DOUBLE_E;
          BufferInsert(&str, c);
        }
        else if (c == '+' || c == '-')
          BufferInsert(&str, c);
        else {
          fprintf(stderr, "ERROR: %s is not valid double.\n", str.buffer);
          free(str.buffer);
          return S_LEXEM_FAIL;
        }
        break;


      case DOUBLE_E:
        if (isdigit(c))
          BufferInsert(&str, c);
        else {
          BufferInsert(&str, '\0');
          ungetc(c, stdin);
          *token = FormToken(DOUBLE, str.buffer);
          t = true;
        }
        break;


      case COMLINE:
        if (c == '\n') {
          *token = FormToken(EOL, NULL);
          t = true;
        }
        break;


      case SLASH:
        if (c == '\'')
          state = COMBLOCK;
        else {
          ungetc(c, stdin);
          *token = FormToken(DIV, NULL);
          t = true;
        }
        break;

      case COMBLOCK:
        if (c == '\'')
          state = COMBLOCK_F;
        break;

      case COMBLOCK_F:
        if (c == '/')
          state = START;
        else if (c == '\'')
          break;
        else
          state = COMBLOCK;
        break;


      case LESS:
        if (c == '>') {
          *token = FormToken(NEQL, NULL);
          t = true;
        }
        else if (c == '=') {
          *token = FormToken(LEQL, NULL);
          t = true;
        }
        else {
          ungetc(c, stdin);
          *token = FormToken(LT, NULL);
          t = true;
        }
        break;


      case GREATER:
        if (c == '=') {
          *token = FormToken(GEQL, NULL);
          t = true;
        }
        else {
          ungetc(c, stdin);
          *token = FormToken(GT, NULL);
          t = true;
        }
        break;

      default:
        // nothing
        break;
    }

    // token should be set - controll and exit function
    if (t) {
      if (*token == NULL)
        return S_MEMORY_ERROR;
      else
        return S_TOKEN_OK;
    }
  }

  // end of input file
  return S_END_OF_FILE;
}
