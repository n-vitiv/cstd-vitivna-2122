#include <stdio.h>
#include <string.h>
#include "../include/configparser.h"
#include "../include/logger.h"
#include "../include/utils.h"

int main()
{
    configParser cParser;
    strcpy(cParser.filePath, ".config");
    configParse(&cParser);
    
    loggerInit(cParser);

    loggerDeinit();
    return 0;
}
