#include "scanner.h"

int SOpenFile(char* path)
{
    SOURCE = fopen(path, "r");

    if(SOURCE == NULL)
    {
        return S_FAILEDOPEN;
    }

    return 0;
}

Token GetToken()
{

}

void SCloseFile()
{
    fclose(SOURCE);
}
