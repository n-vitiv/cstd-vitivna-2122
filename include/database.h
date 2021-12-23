#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>
#include <stdio.h>

MYSQL *conn;

MYSQL* initDatabase();

void closeDatabaseConn(MYSQL *con);

void createDatabase();

void createTable();

void saveData(char* name, int ball_x, int ball_y, int ball_dx, int ball_dy,
                int paddleA, int paddleB, int scoreA, int scoreB);

#endif
