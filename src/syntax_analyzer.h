#ifndef SYNTAX_ANALYZER_H_INCLUDED
#define SYNTAX_ANALYZER_H_INCLUDED

//TBD
enum TokenType {INT, DOUBLE, STRING};
typedef enum TokenType TokType;

typedef struct token{
    TokType Type;
    int ival;
    double fval;
    char* sval;
}Token;

int SynAnalyzer(Token tok);

#endif // SYNTAX_ANALYZER_H_INCLUDED
