#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "syntax_analyzer.h"
#include "symtable.h"
#include "stack_operations.h"

//Martin Stodůlka(xstodu08)
//Ondřej Olšák(xolsak00)
//Michael Schneider(xschne07)
//Marek Kuchynka(xkuchy00)

int main()
{
    Symtable_Init();
    int err;
    err=SyntaxAnalyzer();

    if(!Every_Func_Defed()){
        err = 3;
    }

    GarbageFree();
    Symtable_Destroy();
    stacksDispose();
    return err;
}
