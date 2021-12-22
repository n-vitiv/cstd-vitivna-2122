#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include "configparser.h"

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
} configure;

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
int game_loop();

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

#endif
