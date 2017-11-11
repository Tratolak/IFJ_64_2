#include "code_generator.h"
#include <stdio.h>


FILE *IFJcode17;

/**
 * Otevre soubor pro zapis generovaneho kodu a vlozi do nej uvodni
 * radek i s odradkovanim.
 *
 * @return false - error | true - success
 */
bool fileOpen(){
    IFJcode17 = fopen("IFJcode17","w");
    if (IFJcode17 == NULL){
        return false;
    }else{
        fputs(".IFJcode17\n",IFJcode17);
        return true;
    }
}


/**
 * Zavre soubor pro zapis generovaneho kodu.
 */
void fileClose(){
    fclose(IFJcode17);
}