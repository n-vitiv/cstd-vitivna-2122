#include <stdio.h>
#include <string.h>
#include "../include/configparser.h"
#include "../include/logger.h"
#include "../include/utils.h"
#include "../include/game.h"

int main()
{
    configParser cParser;
    strcpy(cParser.filePath, ".config");
    configParse(&cParser);
    
    loggerInit(cParser);
    setParams(cParser);

    menu();

    loggerDeinit();
    return 0;
}
