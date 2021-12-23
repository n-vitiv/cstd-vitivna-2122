#include "../include/database.h"
#include "../include/logger.h"

MYSQL* initDatabase()
{
    MYSQL *con = mysql_init(NULL);
    if (con == NULL)
    {
        printLog(LOG_ERROR, (char*)"Can`t init database\n");
        return NULL;
    }
    return con;
}

void closeDatabaseConn(MYSQL *con)
{
    if (con != NULL)
    {
        mysql_close(con);
        printLog(LOG_DEBUG, (char*)"Connection with db closed\n");
    }
}

void createDatabase()
{
    if (mysql_real_connect(conn, "localhost", "ponguser", "ponguser",
            NULL, 0, NULL, 0) == NULL)
    {
        printLog(LOG_ERROR, (char*)"%s\n", mysql_error(conn));
        closeDatabaseConn(conn);
        exit(1);
    }
    if (mysql_query(conn, "CREATE DATABASE ponggame"))
    {
        printLog(LOG_ERROR, (char*)"%s\n", mysql_error(conn));
    }
}

void createTable()
{
    if (mysql_query(conn, "USE ponggame"))
    {
        printLog(LOG_ERROR, (char*)"%s", mysql_error(conn));
        closeDatabaseConn(conn);
        exit(1);
    }
    if (mysql_query(conn, "CREATE TABLE save(id INT PRIMARY KEY AUTO_INCREMENT, name VARCHAR(255), \
                            ball_x INT, ball_y INT, ball_dx INT, ball_dy INT, \
                            paddleA INT, paddleB INT, scoreA INT, scoreB INT)"));
    {
        printLog(LOG_ERROR, (char*)"%s\n", mysql_error(conn));
    }
}

void saveData(char* name, int ball_x, int ball_y, int ball_dx, int ball_dy,
                int paddleA, int paddleB, int scoreA, int scoreB)
{
    printLog(LOG_ERROR, "SAVE DATA\n");
    char msg[512];
    snprintf(msg, 512, "INSERT INTO save (id, name, ball_x, ball_y, ball_dx, ball_dy, paddleA, paddleB, scoreA, scoreB) \
            VALUES(DEFAULT, '%s.xml', %d, %d, %d, %d, %d, %d, %d, %d)", 
            name, ball_x, ball_y, ball_dx, ball_dy, paddleA, paddleB, scoreA, scoreB);
    if (mysql_query(conn, msg))
    {
        printLog(LOG_ERROR, (char*)"%s\n", mysql_error(conn));
    }
}
