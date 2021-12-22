#include "gtest/gtest.h"

extern "C"
{
#include "../include/game.h"
#include "../include/configparser.h"
#include "../include/logger.h"
#include "../include/utils.h"
}

TEST(configparser, keyCount)
{
    configParser cParser;
    strcpy(cParser.filePath, ".config");
    configParse(&cParser);
    EXPECT_GT(cParser.keyCount, -1);
}

TEST(getValue, BadValue)
{
    configParser cParser;
    strcpy(cParser.filePath, ".config");
    configParse(&cParser);
    EXPECT_EQ(getValue((char*)"badinput", &cParser), nullptr);
}

TEST(getValue, GoodValue)
{
    configParser cParser;
    strcpy(cParser.filePath, ".config");
    configParse(&cParser);
    EXPECT_STREQ(getValue((char*)"board_width", &cParser), "100");
}

TEST(getTime, time)
{
    EXPECT_NE(getTime(), "");
    EXPECT_NE(getTime(), nullptr);
}

TEST(logger, init)
{
    loggerInit();
    EXPECT_NE(fLog, nullptr);
}

TEST(logger, deinit)
{
    loggerDeinit();
    EXPECT_EQ(fLog, nullptr);
}
