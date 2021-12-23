#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/configparser.h"
#include "../include/logger.h"
#include "../include/utils.h"
#include "../include/game.h"

int main()
{
    system("mkdir saves");
    configParser cParser;
    strcpy(cParser.filePath, ".config");
    configParse(&cParser);
    
    loggerInit(cParser);
    setParams(cParser);

    mainMenu();

    loggerDeinit();
    return 0;
}
