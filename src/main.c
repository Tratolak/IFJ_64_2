#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "syntax_analyzer.h"
#include "symtable.h"

//Martin Stodùlka(xstodu08)
//Ondøej Olšák(xolsak00)
//Michael Schneider(xschne07)
//Marek Kuchynka(xkuchy00)

int main()
{
    Symtable_Init();
    int err;
    //freopen("test.txt", "r", stdin);
    //printf("Hello world!\n");
    err=SyntaxAnalyzer();

    Symtable_Destroy();
    return err;
}
