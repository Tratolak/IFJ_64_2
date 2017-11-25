#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "syntax_analyzer.h"
#include "symtable.h"

int main()
{
    Symtable_Init();
    int err;
    //freopen("test.txt", "r", stdin);
    //printf("Hello world!\n");
    err=SyntaxAnalyzer();
    switch (err){
        case 1:
            return 1;
            break;
        case 2:
            return 2;
            break;

    }
    GarbageFree();
    Symtable_Destroy();
    return err;
}
