#include "scanner.h"

// reserved words from the language  (35)
const char * const Reserved[] = {"as", "asc", "declare", "dim", "do", "double", "else", "end", "chr", "function", "if", "input", "integer", "length", "loop", "print", "return", "scope", "string", "substr", "then", "while", "and", "boolean", "continue", "elseif", "exit", "false", "for", "next", "not", "or", "shared", "static", "true"};

Token* FormToken(TokType type, char* value) {
  Token *token = malloc(sizeof(Token));
  if (token == NULL)
    return NULL;

  token->type = type;
  token->val = NULL;

  if (type == STRING || type == ID || type == KEYWORD) {
    token->val = malloc(strlen(value)+1);
    if (token->val == NULL)
      return NULL;
    strcpy(token->val, value);
  }
  return token;
}

int IsReserved(char* str) {
  for (int i = 0; i < 35; i++) {
    if (strcmp(str, Reserved[i]) == 0)
      return 1;
  }
  return 0;
}

int GetToken(Token **token) {
  int c, i = 0;
  char str[129];
  state = START;

  while ((c = fgetc(fp)) != EOF) {

    if (c == '\r')
      continue;

    c = tolower(c);

    switch (state) {

      case START:

        // skip white
        if (isblank(c))
          break;

        if (c == '\n') {
          *token = FormToken(EOL, NULL);
          return 1;
        }

        // line comment
        if (c == '\'') {
          state = COMLINE;
          break;
        }

        // block comment or dividing
        if (c == '/') {
          if ((c = fgetc(fp)) == '\'')
            state = COMBLOCK;
          else {
            ungetc(c, fp);
            *token = FormToken(DIV, NULL);
            return 1;
          }
          break;
        }

        // identifier/keyword
        if (islower(c) || c == '_')
          state = IDENTIFIER;


        str[i++] = c;
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
          return 1;
        }
        if (i > 128) {
          fprintf(stderr, "Warning: Identificator %s... is too long!\n", str);
          str[i] = '\0';
          ungetc(c, fp);
          *token = FormToken(ID, str);
          return 1;
        }
        break;


      case COMLINE:
        if (c == '\n') {
          *token = FormToken(EOL, NULL);
          return 1;
        }
        break;


      case COMBLOCK:
        if (c == '\'' && (c = fgetc(fp)) == '/')
          state = START;
        break;

      default:
        // nothing yet
        return 1;
    }

  }
  return 0;
}

int SOpenFile(char* path)
{
    SOURCE = fopen(path, "r");

    if(SOURCE == NULL)
    {
        return S_FAILEDOPEN;
    }

    return 0;
}

void SCloseFile()
{
    fclose(SOURCE);
}
