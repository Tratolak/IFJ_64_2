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
  token->val = NULL;

  if (type == STRING || type == ID || type == KEYWORD) {
    token->val = malloc(strlen(value)+1);
    if (token->val == NULL) {
      free(token);
      return NULL;
    }
    strcpy(token->val, value);
  }
  return token;
}

int IsReserved(char* str) {
  for (int i = 0; i < 35; i++) {
    if (strcmp(str, Reserved[i]) == 0)
      return true;
  }
  return false;
}

int GetToken(Token **token) {
  int c, i = 0;
  bool t = false;
  char str[129];
  state = START;

  while ((c = fgetc(fp)) != EOF) {

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
          str[i++] = c;
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

          return S_LEXEM_FAIL;
        }

        break;


      case IDENTIFIER:
        if (isalnum(c) || c == '_')
          str[i++] = c;
        else {
          str[i] = '\0';
          ungetc(c, fp);
          if (IsReserved(str))
            *token = FormToken(KEYWORD, str);
          else
            *token = FormToken(ID, str);
          t = true;
        }
        if (i > 128) {
          fprintf(stderr, "Warning: Identificator %s... is too long!\n", str);
          str[i] = '\0';
          ungetc(c, fp);
          *token = FormToken(ID, str);
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
        if (c == '\'') {
          state = COMBLOCK;
          break;
        }
        else {
          ungetc(c, fp);
          *token = FormToken(DIV, NULL);
          t = true;
        }

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
          ungetc(c, fp);
          *token = FormToken(LT, NULL);
          t = true;
        }


      case GREATER:
        if (c == '=') {
          *token = FormToken(GEQL, NULL);
          t = true;
        }
        else {
          ungetc(c, fp);
          *token = FormToken(GT, NULL);
          t = true;
        }

      default:
        // nothing
        break;
    }

    // token should be set - controll and exit function
    if (t)
      if (*token == NULL)
        return S_MEMORY_ERROR;
      else
        return S_TOKEN_OK;

  }

  // end of input file
  return S_END_OF_FILE;
}
