#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/configparser.h"
#include "../include/logger.h"
#include "../include/utils.h"
#include "../include/game.h"

int main()
{
    srand(time(NULL));
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
