#include <time.h>
#include <stdio.h>

#include "../include/utils.h"
#include "../include/logger.h"

#define MAX_TIME 96

char* getTime()
{
    static char timeStr[MAX_TIME];
    char *format = "%d-%m-%Y %H:%M:%S";
    struct tm *currentTime;
    time_t timer;

    time(&timer);
    currentTime = localtime(&timer);

    strftime(timeStr, MAX_TIME, format, currentTime);
    return timeStr;
}
