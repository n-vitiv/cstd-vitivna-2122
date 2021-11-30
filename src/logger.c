#include <stdio.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "../include/logger.h"
#include "../include/utils.h"

static FILE *fLog;

pthread_mutex_t mutex;

void loggerInit()
{
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("Mutex creation failed.\n");
    }
    struct stat st = {0};

    //creating directory for logs
    if (stat("logs", &st) == -1)
    {
        umask(0);
        mkdir("logs", S_IRWXU | S_IRWXG | S_IRWXO);
    }

    umask(0);
    //opening file for simple logs
    fLog = fopen("logs/logs.log", "w");
    if (fLog == NULL)
    {
        printf("Can`t open file for simple logs.\n");
    }
}

void loggerDeinit()
{  
    if (fLog != NULL)
    {
        fclose(fLog);
    }
    if (pthread_mutex_destroy(&mutex) != 0)
    {
        printf("Mutex destoying failed.\n");
    }
}

void printLog(logLevel level, char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    if (pthread_mutex_lock(&mutex) != 0)
    {
        printf("Can`t lock mutex.\n");
    }

    if (fLog == NULL)
    {
        printf("Can`t print logs.\n");
    }
    else
    {
        switch (level)
        {
            case LOG_DEBUG:
                fprintf(fLog,"DEBUG: %s ", getTime());
                vfprintf(fLog, msg, args);
                fflush(fLog);
                break;
            
            case LOG_ERROR:
                fprintf(fLog,"ERROR: %s ", getTime());
                vfprintf(fLog, msg, args);
                fflush(fLog);
                break;
            
            case LOG_WARNING:
                fprintf(fLog,"WARNING: %s ", getTime());
                vfprintf(fLog, msg, args);
                fflush(fLog);
                break;

            default:
                break;
        }
    }

    va_end(args);
    if (pthread_mutex_unlock(&mutex) != 0)
    {
        printf("Can`t unlock mutex.\n");
    }
}
