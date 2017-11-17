#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "syntax_analyzer.h"

int main()
{
    int err;
    printf("Hello world!\n");
    err=SyntaxAnalyzer();
    switch (err){
        case 1:
            return 1;
            break;
        case 2:
            return 2;
            break;

    }
    return 0;
}
