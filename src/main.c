#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mysql.h>
#include <sys/stat.h>
#include "../include/configparser.h"
#include "../include/logger.h"
#include "../include/utils.h"
#include "../include/game.h"
#include "../include/database.h"

int main()
{
    srand(time(NULL));
    struct stat st = {0};

    //creating directory for logs
    if (stat("saves", &st) == -1)
    {
        umask(0);
        mkdir("saves", S_IRWXU | S_IRWXG | S_IRWXO);
    }

    umask(0);
    configParser cParser;
    strcpy(cParser.filePath, ".config");
    configParse(&cParser);
    
    loggerInit();
    setParams(cParser);

#if !__cplusplus
    conn = initDatabase();
    createDatabase();
    createTable();
#endif
    mainMenu();

#if !__cplusplus
    closeDatabaseConn(conn);
#endif
    loggerDeinit();
    return 0;
}
