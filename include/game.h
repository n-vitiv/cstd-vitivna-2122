#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include "configparser.h"

#define MODE_BUF 5

#if __cplusplus
extern "C" {
#endif

typedef struct
{
	int x, y;       // position on the screen
	int dx, dy;     // movement vector
} ballData;

typedef struct
{
	int a, b;
} paddleData;

typedef struct
{
	int scoreA, scoreB;
} scoreCount;

typedef struct
{
	int paddleLength;
	int boardWidth;
	int boardHeight;
	char mode[MODE_BUF];
} configure;

typedef enum
{
	MAN_VS_MAN,
	MAN_VS_AI,
	AI_VS_AI
} gameMode;

typedef enum
{
	EASY,
	NORM,
	HARD
} aiMode;

/**
 * @brief function for printing game menu to console
 * 
 * @return void 
 */
void menu();

/**
 * 
 */
void modeChoice(int mode);

/**
 * 
 */
void manVsMan();

/**
 * 
 */
void manVsAI();

/**
 * 
 */
void AIvsAI();

/**
 * 
 */
void closeScreen(WINDOW *w);

/**
 * 
 */
int game_loop(gameMode mode);

/**
 * 
 */
void initial_print();

/**
 * 
 */
void draw_scoreline();

/**
 * 
 */
void setParams(configParser cParser);

/**
 *
 */
void mainMenu();

/**
 * 
 */
void mainMenuChoice(int choice);

/**
 * 
 */
void saveGame();

/**
 * 
 */
void loadGame();

/**
 *
 */
void createSaveAtXML(char *name);

/**
 *
 */
void parseXMLSave(char *name);

/**
 *
 */
void initScreen(WINDOW *w);

/**
 * 
 */
void play(gameMode mode);

/**
 * 
 */
aiMode strAiModeToInt(char *mode);

#if __cplusplus
}
#endif

#if !__cplusplus
void parseDBSave(char *name);
#endif

#endif
