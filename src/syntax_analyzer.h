#ifndef SYNTAX_ANALYZER_H_INCLUDED
#define SYNTAX_ANALYZER_H_INCLUDED

//TBD
enum TokenType {INT, DOUBLE, STRING};
typedef enum TokenType TokType;

typedef struct token{
    TokType type;
    char* val;
}Token;

int SynAnalyzer();

#endif // SYNTAX_ANALYZER_H_INCLUDED
