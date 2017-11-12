#include "code_generator.h"
#include "stack_operations.h"
#include <stdio.h>
#include <malloc.h>

FILE *IFJcode17;
tStack *levelStack;

/**
 * Otevre soubor pro zapis generovaneho kodu a vlozi do nej uvodni
 * radek i s odradkovanim.
 *
 * @return false - error | true - ok
 */
bool fileOpen() {
    IFJcode17 = fopen("IFJcode17", "w");
    if (IFJcode17 == NULL) {
        return false;
    } else {
        fputs(".IFJcode17\n", IFJcode17);
        levelStack = (tStack *) malloc(sizeof(tStack));
        return true;
    }
}

/**
 * Vestavena fce na zjisteni delky retezce.
 */
void inbuildLenght() {
    fputs("STRLEN /*vysledek (int)*/ /*retezec (string)*/\n", IFJcode17);
}

/**
 * Vlozeni zacatku i konce cyklu.
 */
void loop() {
    char stackTmp;
    if (/*begin of while*/) {
        stackPush(levelStack, /*begin of while*/);
        fputs("LABEL /*any name*/\n", IFJcode17);
        fputs("/*condition*/ /*when*/ /*endOfWhileCycle*/\n", IFJcode17);
    } else {
        stackTop(levelStack, &stackTmp);
        if (stackTmp == /*end of while*/) {
            fputs("JMP /*any name*/\n", IFJcode17);
            fputs("LABEL /*any ENDname*/\n", IFJcode17);
        }
    }
}
/**
 * Vlozeni uplne i castecne pominky.
 */
void ifCommand() {
    char stackTmp;
    //Zacatek podminky
    if (/*begin of if*/) {
        stackPush(levelStack, /*begin of while*/);
        fputs("/*conditional jump to else/end of if*/\n", IFJcode17);
    // Uplna podminka ELSE
    } else if (stackTmp == /*else*/) {
        stackTop(levelStack, &stackTmp);
        if (stackTmp == /*begin of if*/) {
            fputs("JMP /*end if*/\n", IFJcode17);
            fputs("LABEL /*else*/\n", IFJcode17);
            //Konec podminky
        } else if (/*end if*/) {
            fputs("LABEL /*end if*/\n", IFJcode17);
        }
    }
}

void function(){

}
/**
 * Zavre soubor pro zapis generovaneho kodu.
 */
void fileClose() {
    fclose(IFJcode17);
}