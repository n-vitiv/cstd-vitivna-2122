#ifndef LOGGER_H
#define LOGGER_H

#if __cplusplus
extern "C" {
#endif

extern FILE *fLog;

typedef enum
{
    LOG_DEBUG,
    LOG_ERROR,
    LOG_WARNING,
    LOG_W3C
} logLevel;

/**
 * @brief initialization of logger unit
 * 
 */
void loggerInit();

/**
 * @brief deinitialization of logger unit
 * 
 */
void loggerDeinit();

/**
 * @brief printing logs
 * 
 * @param level logger level 
 * @param msg message which will be logged
 */
void printLog(logLevel level, char *msg, ...);

#if __cplusplus
}
#endif

#endif
