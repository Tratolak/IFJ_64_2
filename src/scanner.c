#include "scanner.h"

// reserved words from the language  (35)
const char * const Reserved[] = {"as", "asc", "declare", "dim", "do", "double",
"else", "end", "chr", "function", "if", "input", "integer", "length", "loop",
"print", "return", "scope", "string", "substr", "then", "while", "and",
"boolean", "continue", "elseif", "exit", "false", "for", "next", "not", "or",
"shared", "static", "true"};


enum enState {START, IDENTIFIER, COMLINE, COMBLOCK, COMBLOCK_F, LESS, GREATER,
              SLASH, INT_Z, INT, DOUBLE_NN, DOUBLE_N, DOUBLE_ES, DOUBLE_EN,
              DOUBLE_E, EXCLAMATION, STR, ESCAPE
} state;



Token* FormToken(TokType type, char* value) {
  Token *token = malloc(sizeof(Token));
  if (token == NULL)
    return NULL;

  token->type = type;
  token->val = value;

  return token;
}


void FreeToken(Token **token) {
  TokType type = (*token)->type;
  if (type == INTEGER || type == DOUBLE || type == STRING
      || type == ID || type == KEYWORD)
    free((*token)->val);
  free(*token);
  *token = NULL;
}


bool IsReserved(char* str) {
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


bool BufferInsert(DynamicBuffer *b, char c) {
  if (b->used == b->size) {
    b->size += 32;
    b->buffer = (char *)realloc(b->buffer, b->size + sizeof(char) * 32);
    if (b->buffer == NULL)
      return false;
  }
  b->buffer[b->used++] = c;
  return true;
}


int GetToken(Token **token) {
  int c, count, sym;
  bool t = false;
  DynamicBuffer str;
  state = START;

  while ((c = getchar()) != EOF) {

    if (c == '\r')
      continue;

    switch (state) {

      case START:

        // skip white
        if (isblank(c))
          break;

        // new line
        else if (c == '\n') {
          *token = FormToken(EOL, NULL);
          t = true;
        }

        // identifier/keyword
        else if (isalpha(c) || c == '_') {
          state = IDENTIFIER;
          if (!BufferInit(&str, 32))
            return S_MEMORY_ERROR;
          if (!BufferInsert(&str, tolower(c)))
            return S_MEMORY_ERROR;
        }

        else if (isdigit(c)) {
          if (!BufferInit(&str, 32))
            return S_MEMORY_ERROR;
          if (c == '0') {
            state = INT_Z;
          } else {
            state = INT;
            if (!BufferInsert(&str, c))
              return S_MEMORY_ERROR;
          }
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

        // integer dividing
        else if (c == '\\') {
          *token = FormToken(IDIV, NULL);
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

        // exclamation mark > maybe string?
        else if (c == '!')
          state = EXCLAMATION;

        // comma
        else if (c == ',') {
          *token = FormToken(COMMA, NULL);
          t = true;
        }

        // semicolon
        else if (c == ';') {
          *token = FormToken(SEMICOLON, NULL);
          t = true;
        }

        // percent sign
        else if (c == '%') {
          *token = FormToken(PERCENT, NULL);
          t = true;
        }

        // left bracket
        else if (c == '(') {
          *token = FormToken(LBRACKET, NULL);
          t = true;
        }

        // right bracket
        else if (c == ')') {
          *token = FormToken(RBRACKET, NULL);
          t = true;
        }

        // unexpected character
        else {
          fprintf(stderr, "ERROR: unexpected symbol \"%c\".\n", c);
          return S_LEXEM_FAIL;
        }

        break;


      case IDENTIFIER:
        if (isalnum(c) || c == '_') {
          if (!BufferInsert(&str, tolower(c)))
            return S_MEMORY_ERROR;
        }
        else {
          if (!BufferInsert(&str, '\0'))
            return S_MEMORY_ERROR;
          ungetc(c, stdin);
          if (IsReserved(str.buffer))
            *token = FormToken(KEYWORD, str.buffer);
          else
            *token = FormToken(ID, str.buffer);
          t = true;
        }
        break;


      case INT_Z:
        if (isdigit(c)) {
          if (c == '0')
            break;
          else {
            state = INT;
            if (!BufferInsert(&str, c))
              return S_MEMORY_ERROR;
          }
        }
        else if (c == '.') {
          state = DOUBLE_NN;
          if (!BufferInsert(&str, '0') || !BufferInsert(&str, c))
            return S_MEMORY_ERROR;
        }
        else if (c == 'e' || c == 'E') {
          state = DOUBLE_ES;
          if (!BufferInsert(&str, '0') || !BufferInsert(&str, 'e'))
            return S_MEMORY_ERROR;
        }
        else {
          if (!BufferInsert(&str, '0') || !BufferInsert(&str, '\0'))
            return S_MEMORY_ERROR;
          ungetc(c, stdin);
          *token = FormToken(INTEGER, str.buffer);
          t = true;
        }
        break;

      case INT:
        if (isdigit(c)) {
          if (!BufferInsert(&str, c))
            return S_MEMORY_ERROR;
        }
        else if (c == '.') {
          state = DOUBLE_NN;
          if (!BufferInsert(&str, c))
            return S_MEMORY_ERROR;
        }
        else if (c == 'e' || c == 'E') {
          state = DOUBLE_ES;
          if (!BufferInsert(&str, 'e'))
            return S_MEMORY_ERROR;
        }
        else {
          if (!BufferInsert(&str, '\0'))
            return S_MEMORY_ERROR;
          ungetc(c, stdin);
          *token = FormToken(INTEGER, str.buffer);
          t = true;
        }
        break;


      case DOUBLE_NN:
        if (isdigit(c)) {
          state = DOUBLE_N;
          if (!BufferInsert(&str, c))
            return S_MEMORY_ERROR;
        }
        else {
          if (c == '\n')
            fprintf(stderr, "ERROR: Unexpected EOL after %s.\n", str.buffer);
          else
            fprintf(stderr, "ERROR: %s%c is not valid double.\n", str.buffer,c);
          free(str.buffer);
          return S_LEXEM_FAIL;
        }
        break;

      case DOUBLE_N:
        if (isdigit(c)) {
          if (!BufferInsert(&str, c))
            return S_MEMORY_ERROR;
        }
        else if (c == 'e' || c == 'E') {
          state = DOUBLE_ES;
          if (!BufferInsert(&str, 'e'))
            return S_MEMORY_ERROR;
        }
        else {
          if (!BufferInsert(&str, '\0'))
            return S_MEMORY_ERROR;
          ungetc(c, stdin);
          *token = FormToken(DOUBLE, str.buffer);
          t = true;
        }
        break;


      case DOUBLE_ES:
        if (isdigit(c)) {
          state = DOUBLE_E;
          if (!BufferInsert(&str, c))
            return S_MEMORY_ERROR;
        }
        else if (c == '+' || c == '-') {
          state = DOUBLE_EN;
          if (!BufferInsert(&str, c))
            return S_MEMORY_ERROR;
        }
        else {
          fprintf(stderr, "ERROR: %s is not valid double.\n", str.buffer);
          free(str.buffer);
          return S_LEXEM_FAIL;
        }
        break;

      case DOUBLE_EN:
        if (isdigit(c)) {
          state = DOUBLE_E;
          if (!BufferInsert(&str, c))
            return S_MEMORY_ERROR;
        }
        else {
          if (c == '\n')
            fprintf(stderr, "ERROR: Unexpected EOL after %s.\n", str.buffer);
          else
            fprintf(stderr, "ERROR: %s%c is not valid double.\n", str.buffer,c);
          free(str.buffer);
          return S_LEXEM_FAIL;
        }
        break;

      case DOUBLE_E:
        if (isdigit(c)) {
          if (!BufferInsert(&str, c))
            return S_MEMORY_ERROR;
        }
        else {
          if (!BufferInsert(&str, '\0'))
            return S_MEMORY_ERROR;
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


      case EXCLAMATION:
        if (c == '\"') {
          if (!BufferInit(&str, 32))
            return S_MEMORY_ERROR;
          state = STR;
        }
        else {
          state = START;
          ungetc(c, stdin);
          fprintf(stderr, "ERROR: unexpected symbol \"!\".\n");
          return S_LEXEM_FAIL;
        }
        break;

      case STR:
        if (c == '\"') {
          if (!BufferInsert(&str, '\0'))
            return S_MEMORY_ERROR;
          *token = FormToken(STRING, str.buffer);
          t = true;
        }
        else if (c == '\n') {
          fprintf(stderr, "ERROR: unexpected EOL in string \"%s\".\n", str.buffer);
          free(str.buffer);
          return S_LEXEM_FAIL;
        }
        else if (c == '\\') {
          state = ESCAPE;
          count = 0;
          sym = 0;
        }
        else if (c <= 32 || c == 35 || c == 92) {
          char esc[4];
          sprintf(esc, "%03d", c);
          if (!BufferInsert(&str, '\\') || !BufferInsert(&str, esc[0]) ||
              !BufferInsert(&str, esc[1]) || !BufferInsert(&str, esc[2]))
            return S_MEMORY_ERROR;
        }
        else
          if (!BufferInsert(&str, c))
            return S_MEMORY_ERROR;
        break;

      case ESCAPE:
        if (isdigit(c)) {
          // "\number"
          count++;
          switch (count) {
            case 1:
              sym = c * 100;
              if (!BufferInsert(&str, '\\') || !BufferInsert(&str, c))
                return S_MEMORY_ERROR;
              break;
            case 2:
              sym = sym + c * 10;
              if (!BufferInsert(&str, c))
                return S_MEMORY_ERROR;
              break;
            case 3:
              sym = sym + c;
              if (!BufferInsert(&str, c))
                return S_MEMORY_ERROR;
              break;
          }
          if (count == 3) {
            if (sym < 256) {
              state = STR;
            }
            else {
              fprintf(stderr, "ERROR: too big number in escape sequence \"%s\\%d\".\n", str.buffer, sym);
              free(str.buffer);
              return S_LEXEM_FAIL;
            }
          }
        }
        else if (count == 0) {
          // "\n \t \" \\"
          state = STR;
          if (c == '"') {
            if (!BufferInsert(&str, '"'))
              return S_MEMORY_ERROR;
          }
          else {
            if (!BufferInsert(&str, '\\') || !BufferInsert(&str, '0'))
              return S_MEMORY_ERROR;
            if (c == 'n') {
              if (!BufferInsert(&str, '1') || !BufferInsert(&str, '0'))
                return S_MEMORY_ERROR;
            }
            else if (c == 't') {
              if (!BufferInsert(&str, '0') || !BufferInsert(&str, '9'))
                return S_MEMORY_ERROR;
            }
            else if (c == '\\') {
              if (!BufferInsert(&str, '9') || !BufferInsert(&str, '2'))
                return S_MEMORY_ERROR;
            }
            else {
              fprintf(stderr, "ERROR: unknown escape sequence \"\\%c\" in \"%s\".\n", c, str.buffer);
              free(str.buffer);
              return S_LEXEM_FAIL;
            }
          }
        }
        else {
          fprintf(stderr, "ERROR: unknown escape sequence \"\\%d\" in \"%s\".\n", sym, str.buffer);
          free(str.buffer);
          return S_LEXEM_FAIL;
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
