#include "../include/game.h"
#include "../include/logger.h"
#if !__cplusplus
#include "../include/database.h"
#endif

#include <string.h>
#include <dirent.h> 
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>

#define FRAMETIME 80000
#define SAVE_NAME 16

int user_input;
paddleData paddle;
ballData ball;
scoreCount score;
configure conf;
aiMode ai;
int counter;
int ball_delay;
int isLoading = 0;

void mainMenu()
{
    system("clear");
    int mode = 1;
    printf("======================================================================================\n");
    printf("=                                  Ping Pong game                                    =\n");
    printf("=                                                                                    =\n");
    printf("======================================================================================\n");
    printf("= Game modes                                                                         =\n");
    printf("= 1. Play game                                                                       =\n");
    printf("= 2. Load game                                                                       =\n");
    printf("= 3. Quit game                                                                       =\n");
    printf("======================================================================================\n");
    printf("= Enter your choice (1-3): ");
    scanf("%d", &mode);
    mainMenuChoice(mode);
}

void menu()
{
    score.scoreA = 0;
    score.scoreB = 0;
    system("clear");
    int mode = 1;
    ai = strAiModeToInt(conf.mode);
    printLog(LOG_DEBUG, (char*)"ai mode: %s\n", conf.mode);
    printf("======================================================================================\n");
    printf("=                                  Ping Pong game                                    =\n");
    printf("=                                                                                    =\n");
    printf("======================================================================================\n");
    printf("= Game modes                                                                         =\n");
    printf("= 1. Man vs Man                                                                      =\n");
    printf("= 2. Man vs AI                                                                       =\n");
    printf("= 3. AI vs AI                                                                        =\n");
    printf("======================================================================================\n");
    printf("= Enter your choice (1-3): ");
    scanf("%d", &mode);
    modeChoice(mode);
}

void mainMenuChoice(int choice)
{
    switch (choice)
    {
    case 1:
        system("clear");
        printLog(LOG_DEBUG, (char*)"choice: play game\n");
        menu();
        break;
    case 2:
        system("clear");
        printLog(LOG_DEBUG, (char*)"choice: load game\n");
        loadGame();
        break;
    case 3:
        printf("Leaving game.\n");
        sleep(3);
        system("clear");
        exit(1);
    default:
        printf("You entered bad mode. Please try again after 5 seconds.\n");
        sleep(5);
        system("clear");
        mainMenu();
        break;
    }
}

void modeChoice(int mode)
{
    switch (mode)
    {
    case 1:
        system("clear");
        printLog(LOG_DEBUG, (char*)"mode: man vs man\n");
        play(MAN_VS_MAN);
        break;
    
    case 2:
        system("clear");
        printLog(LOG_DEBUG, (char*)"mode: man vs ai\n");
        play(MAN_VS_AI);
        break;

    case 3:
        system("clear");
        printLog(LOG_DEBUG, (char*)"mode: ai vs ai\n");
        play(AI_VS_AI);
        break;

    default:
        printf("You entered bad mode. Please try again after 5 seconds.\n");
        sleep(5);
        system("clear");
        menu();
        break;
    }
}

void play(gameMode mode)
{
    WINDOW *w = initscr();
    noecho();
    curs_set(FALSE);
    cbreak();
    nodelay(w, TRUE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    user_input = 0;
    while (score.scoreA < 10 && score.scoreB < 10 && user_input != 'q')
    {
        initial_print();
        draw_scoreline();
        int loop;
        while (user_input != 'q')
        {
            user_input = getch();
            loop = game_loop(mode);
            if (loop == 'a')
            {
                score.scoreA++;
                draw_scoreline();
                break;
            }
            else if (loop == 'b')
            {
                score.scoreB++;
                draw_scoreline();
                break;
            }
        }
        usleep(1000000);
    }
    
    closeScreen(w);

    if (user_input == 'q')
    {
        closeScreen(w);
        isLoading = 0;
        saveGame();
        mainMenu();
    }

    printf("========================== WINNER ================================\n");
    if (score.scoreA >= 10)
    {
        printf("========================= PLAYER A ===============================\n");
    }
    else if (score.scoreB >= 10)
    {
        printf("========================= PLAYER B ===============================\n");
    }
    printf("==================================================================\n");
    sleep(5);
    mainMenu();
}

void manVsMan()
{
    // Edit paddle A
    if (user_input == 'w' && paddle.a > 4)
    {
        attroff(COLOR_PAIR(1));
        mvprintw(paddle.a + conf.paddleLength / 2, 1, "  ");
        attron(COLOR_PAIR(1));
        paddle.a--;
        mvprintw(paddle.a - conf.paddleLength / 2, 1, " ");
    }
    else if (user_input == 's' && paddle.a < conf.boardHeight - 3)
    {
        attroff(COLOR_PAIR(1));
        mvprintw(paddle.a - conf.paddleLength / 2, 1, " ");
        attron(COLOR_PAIR(1));
        paddle.a++;
        mvprintw(paddle.a + conf.paddleLength / 2, 1, " ");
    }
    
    // Edit paddle B 
    if (user_input == 'o' && paddle.b > 4)
    {
        attroff(COLOR_PAIR(1));
        mvprintw(paddle.b + conf.paddleLength / 2, conf.boardWidth - 1, "  ");
        attron(COLOR_PAIR(1));
        paddle.b--;
        mvprintw(paddle.b - conf.paddleLength / 2, conf.boardWidth - 1, " ");
    }
    else if (user_input == 'l' && paddle.b < conf.boardHeight - 3)
    {
        attroff(COLOR_PAIR(1));
        mvprintw(paddle.b - conf.paddleLength / 2, conf.boardWidth - 1, " ");
        attron(COLOR_PAIR(1));
        paddle.b++;
        mvprintw(paddle.b + conf.paddleLength / 2, conf.boardWidth - 1, " ");
    }
}

void manVsAI()
{
    int random;
    if (ai == NORM)
        random = rand() % 100 - 92;
    else if (ai == HARD)
        random = rand() % 100 - 90;
    else
        random = rand() % 100 - 95;

    if (user_input == 'w' && paddle.a > 4)
    {
        attroff(COLOR_PAIR(1));
        mvprintw(paddle.a + conf.paddleLength / 2, 1, "  ");
        attron(COLOR_PAIR(1));
        paddle.a--;
        mvprintw(paddle.a - conf.paddleLength / 2, 1, " ");
    }
    else if (user_input == 's' && paddle.a < conf.boardHeight - 3)
    {
        attroff(COLOR_PAIR(1));
        mvprintw(paddle.a - conf.paddleLength / 2, 1, " ");
        attron(COLOR_PAIR(1));
        paddle.a++;
        mvprintw(paddle.a + conf.paddleLength / 2, 1, " ");
    }

    if (random > 0 && ball.dx > 0)
    {
        if (ball.y > paddle.b && paddle.b < conf.boardHeight - 3)
        {
            attroff(COLOR_PAIR(1));
            mvprintw(paddle.b - conf.paddleLength / 2, conf.boardWidth - 1, " ");
            attron(COLOR_PAIR(1));
            paddle.b++;
            mvprintw(paddle.b + conf.paddleLength / 2, conf.boardWidth - 1, " ");
        }
        else if (ball.y < paddle.b && paddle.b > 4)
        {
            attroff(COLOR_PAIR(1));
            mvprintw(paddle.b + conf.paddleLength / 2, conf.boardWidth - 1, " ");
            attron(COLOR_PAIR(1));
            paddle.b--;
            mvprintw(paddle.b - conf.paddleLength / 2, conf.boardWidth - 1, " ");
        }
    }
}

void AIvsAI()
{
    int random;
    if (ai == NORM)
        random = rand() % 100 - 92;
    else if (ai == HARD)
        random = rand() % 100 - 90;
    else
        random = rand() % 100 - 95;
    if (random > 0 && ball.dx < 0)
    {
        if (ball.y < paddle.a && paddle.a > 4)
        {
            attroff(COLOR_PAIR(1));
            mvprintw(paddle.a + conf.paddleLength / 2, 1, "  ");
            attron(COLOR_PAIR(1));
            paddle.a--;
            mvprintw(paddle.a - conf.paddleLength / 2, 1, " ");
        }
        else if (ball.y > paddle.a && paddle.a < conf.boardHeight - 3)
        {
            attroff(COLOR_PAIR(1));
            mvprintw(paddle.a - conf.paddleLength / 2, 1, " ");
            attron(COLOR_PAIR(1));
            paddle.a++;
            mvprintw(paddle.a + conf.paddleLength / 2, 1, " ");
        }
    }    
    if (random > 0 && ball.dx > 0)
    {
        if (ball.y > paddle.b && paddle.b < conf.boardHeight - 3)
        {
            attroff(COLOR_PAIR(1));
            mvprintw(paddle.b - conf.paddleLength / 2, conf.boardWidth - 1, " ");
            attron(COLOR_PAIR(1));
            paddle.b++;
            mvprintw(paddle.b + conf.paddleLength / 2, conf.boardWidth - 1, " ");
        }
        else if (ball.y < paddle.b && paddle.b > 4)
        {
            attroff(COLOR_PAIR(1));
            mvprintw(paddle.b + conf.paddleLength / 2, conf.boardWidth - 1, " ");
            attron(COLOR_PAIR(1));
            paddle.b--;
            mvprintw(paddle.b - conf.paddleLength / 2, conf.boardWidth - 1, " ");
        }
    }
}

void closeScreen(WINDOW *w)
{
    // Closing screen sequence
    nocbreak();
    nodelay(w, FALSE);
    echo();
    curs_set(TRUE);
    endwin();
}

int game_loop(gameMode mode)
{
    if (mode == MAN_VS_MAN)
    {
        manVsMan();
    }
    else if (mode == MAN_VS_AI)
    {
        manVsAI();
    }
    else if (mode == AI_VS_AI)
    {
        AIvsAI();
    }

    // Handling the ball occasionally
    if (counter == ball_delay-1) 
    {
        // Remove Ball 
        attroff(COLOR_PAIR(1));
        mvprintw(ball.y, ball.x, "  ");

        // Update Ball location
        if (ball.y <= 2 || ball.y >= conf.boardHeight - 1)
            ball.dy = -ball.dy;

        if (ball.x == 2 && ball.y <= paddle.a + conf.paddleLength / 2 && ball.y >= paddle.a - conf.paddleLength / 2) 
            ball.dx = -ball.dx;

        if (ball.x == conf.boardWidth - 4 && ball.y <= paddle.b + conf.paddleLength / 2 && ball.y >= paddle.b - conf.paddleLength / 2)
            ball.dx = -ball.dx; 
            
        ball.x += ball.dx;
        ball.y += ball.dy;

        if (ball.x <= 0)
            return 'b';
        else if (ball.x >= conf.boardWidth)
            return 'a';
    }
    counter++;
    counter %= ball_delay;

    // Draw Ball
    attron(COLOR_PAIR(1));
    mvprintw(ball.y, ball.x, "  ");
    usleep(FRAMETIME / 10);
    return 0;
}

void initial_print()
{
    clear();

    if (!isLoading)
    {
        printLog(LOG_DEBUG, (char*)"initial_print: isLoading true\n");
        // Initializing Values
        paddle.a = conf.boardHeight / 2;
        paddle.b = conf.boardHeight / 2;
        ball.x = conf.boardWidth / 2;
        ball.y = conf.boardHeight / 2;
        ball.dx = -1;
        ball.dy = 1;
        user_input = 0;
        counter = 0;
        ball_delay = 10;
    }
    isLoading = 0;
    // Draw ball
    attron(COLOR_PAIR(1));
    mvprintw(ball.y, ball.x, "  ");

    // Draw Paddle A
    for (int i = 0; i < conf.paddleLength; i++) 
        mvprintw(paddle.a - conf.paddleLength / 2 + i, 1, " ");

    // Draw Paddle B
    for (int i = 0; i< conf.paddleLength; i++) 
        mvprintw(paddle.b - conf.paddleLength / 2 + i, conf.boardWidth - 1, " ");
    
    refresh();
    return;
}

void draw_scoreline()
{
    // Drawing user names
    attroff(COLOR_PAIR(1));
    char *a = (char*)malloc(10);
    sprintf(a, "User A: %d", score.scoreA);
    mvprintw(0, 1, a);
    sprintf(a, "User B: %d", score.scoreB);
    mvprintw(0, conf.boardWidth-11, a);
    mvprintw(0, conf.boardWidth / 2 - 1, "||");

    // Draw lower border
    attron(COLOR_PAIR(1));
    for (int i = 0; i < conf.boardWidth; i++)
    {
        mvprintw(1, i, " ");
    }
    attron(COLOR_PAIR(1));
    for (int i = 0; i < conf.boardWidth; i++)
    {
        mvprintw(conf.boardHeight, i, " ");
    }
    
    refresh();
    return;
}

void setParams(configParser cParser)
{
    conf.boardHeight = atoi(getValue((char*)"board_height", &cParser));
    if (conf.boardHeight <= 0)
    {
        conf.boardHeight = 24;
    }

    conf.boardWidth = atoi(getValue((char*)"board_width", &cParser));
    if (conf.boardWidth <= 0)
    {
        conf.boardWidth = 80;
    }

    conf.paddleLength = atoi(getValue((char*)"tool_size", &cParser));
    if (conf.paddleLength <= 0)
    {
        conf.paddleLength = 5;
    }

    strcpy(conf.mode, getValue((char*)"ai_mode", &cParser));
    if (conf.mode == NULL)
    {
        strcpy(conf.mode, "easy");
    }
}

aiMode strAiModeToInt(char *mode)
{
    if (!strcmp(mode, "easy"))
    {
        return EASY;
    }
    else if (!strcmp(mode, "norm"))
    {
        return NORM;
    }
    else if (!strcmp(mode, "hard"))
    {
        return HARD;
    }
    else
    {
        return EASY;
    }
}

void saveGame()
{
    system("clear");
    int choice = 1;
    printf("======================================================================================\n");
    printf("=                                  Ping Pong game                                    =\n");
    printf("=                                                                                    =\n");
    printf("======================================================================================\n");
    printf("= 1. Save game                                                                       =\n");
    printf("= 2. Leave without save                                                              =\n");
    printf("======================================================================================\n");
    scanf("%d", &choice);
    switch (choice)
    {
        case 1:
        {
            system("clear");
            printf("Enter save name: ");
            char name[32] = "saves/";
            char filename[SAVE_NAME];
            scanf("%s", filename);
            printLog(LOG_DEBUG, (char*)"Creating new save: %s\n", filename);
#if !__cplusplus
            saveData(filename, ball.x, ball.y, ball.dx, ball.dy, paddle.a, paddle.b, score.scoreA, score.scoreB);
#endif
            createSaveAtXML(strcat(name, filename));
            break;
        }
        case 2:
            mainMenu();
            break;
        default:
            printf("You entered bad mode. Please try again after 5 seconds.\n");
            sleep(5);
            system("clear");
            saveGame();
            break;
    }
}

void loadGame()
{
    DIR *d;
    struct dirent *dir;
    int counter = 1, choice;
    d = opendir("saves");
    char saveName[32];
    
    if (d) 
    {
        while ((dir = readdir(d)) != NULL) 
        {
            if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
                continue;
            printf("%d. %s\n",counter, dir->d_name);
            counter++;
        }
        closedir(d);
    }
    printf("%d. go to main menu\n", counter);
    printf("Enter needed save (1-%d): ", counter);
    scanf("%d", &choice);
    if (choice > counter)
    {
        printf("Bad input.\nGoing back to main menu\n.");
        sleep(3);
        mainMenu();
    }
    else if (choice == counter)
    {
        mainMenu();
    }

    d = opendir("saves");
    for (int i = 0; i < choice + 2; ++i)
    {
        dir = readdir(d);
#if __cplusplus
        strcpy(saveName, "saves/");
        strcat(saveName, dir->d_name);
#else
        strcpy(saveName, dir->d_name);
#endif
    }
    closedir(d);
    printLog(LOG_DEBUG, (char*)"Loading game: %s\n", saveName);
    isLoading = 1;
#if __cplusplus
    parseXMLSave(saveName);
#else
    parseDBSave(saveName);
#endif
}

void createSaveAtXML(char *name)
{
    FILE *f = fopen(strcat(name, ".xml"), "w");

    //save ball data
    fprintf(f, "<ball>\n");
    fprintf(f, "\t<ball_x>%d</ball_x>\n", ball.x);
    fprintf(f, "\t<ball_y>%d</ball_y>\n", ball.y);
    fprintf(f, "\t<ball_dx>%d</ball_dx>\n", ball.dx);
    fprintf(f, "\t<ball_dy>%d</ball_dy>\n", ball.dy);
    fprintf(f, "</ball>\n");

    //save player1 data
    fprintf(f, "<paddleA>%d</paddleA>\n", paddle.a);
    fprintf(f, "<scoreA>%d</scoreA>\n", score.scoreA);

    //save player2 data
    fprintf(f, "<paddleB>%d</paddleB>\n", paddle.b);
    fprintf(f, "<scoreB>%d</scoreB>\n", score.scoreB);

    fclose(f);
}

void parseXMLSave(char *name)
{
    // Initializing Values by default
    paddle.a = conf.boardHeight / 2;
    paddle.b = conf.boardHeight / 2;
    ball.x = conf.boardWidth / 2;
    ball.y = conf.boardHeight / 2;
    ball.dx = -1;
    ball.dy = 1;
    user_input = 0;
    counter = 0;
    ball_delay = 10;
    
    FILE *fp = fopen(name, "r");
    if (fp == NULL)
    {
        printLog(LOG_ERROR, (char*)"can`t load file: %s\n", name);
        return;
    }

    char *buffer = NULL;
    size_t buflen = 0;
    char *target = NULL;
    char *start, *end;

    while (getline(&buffer, &buflen, fp) != -1)
    {
        if (strstr(buffer, "<ball_x>") != NULL)
        {
            start = strstr(buffer, "<ball_x>");
            start += strlen("<ball_x>");
            end = strstr(start, "</ball_x>");
            if (end != NULL)
            {
                target = ( char * )malloc( end - start + 1 );
                memcpy( target, start, end - start );
                target[end - start] = '\0';
                ball.x = atoi(target);
            }
        }
        else if (strstr(buffer, "ball_y") != NULL)
        {
            start = strstr(buffer, "<ball_y>");
            start += strlen("<ball_y>");
            end = strstr( start, "</ball_y>" );
            if (end != NULL)
            {
                target = ( char * )malloc( end - start + 1 );
                memcpy( target, start, end - start );
                target[end - start] = '\0';
                ball.y = atoi(target);
            }
        }
        else if (strstr(buffer, "ball_dx") != NULL)
        {
            start = strstr(buffer, "<ball_dx>");
            start += strlen("<ball_dx>");
            end = strstr(start, "</ball_dx>");
            if (end != NULL)
            {
                target = ( char * )malloc( end - start + 1 );
                memcpy( target, start, end - start );
                target[end - start] = '\0';
                ball.dx = atoi(target);
            }
        }
        else if (strstr(buffer, "ball_dy") != NULL)
        {
            start = strstr(buffer, "<ball_dy>");
            start += strlen("<ball_dy>");
            end = strstr(start, "</ball_dy>");
            if (end != NULL)
            {
                target = ( char * )malloc( end - start + 1 );
                memcpy( target, start, end - start );
                target[end - start] = '\0';
                ball.dy = atoi(target);
            }
        }
        else if (strstr(buffer, "paddleA") != NULL)
        {
            start = strstr(buffer, "<paddleA>");
            start += strlen("<paddleA>");
            end = strstr(start, "</paddleA>");
            if (end != NULL)
            {
                target = ( char * )malloc( end - start + 1 );
                memcpy( target, start, end - start );
                target[end - start] = '\0';
                paddle.a = atoi(target);
            }
        }
        else if (strstr(buffer, "paddleB") != NULL)
        {
            start = strstr(buffer, "<paddleB>");
            start += strlen("<paddleB>");
            end = strstr(start, "</paddleB>");
            if (end != NULL)
            {
                target = ( char * )malloc( end - start + 1 );
                memcpy( target, start, end - start );
                target[end - start] = '\0';
                paddle.b = atoi(target);
            }
        }
        else if (strstr(buffer, "scoreA") != NULL)
        {
            start = strstr(buffer, "<scoreA>");
            start += strlen("<scoreA>");
            end = strstr(start, "</scoreA>");
            if (end != NULL)
            {
                target = ( char * )malloc( end - start + 1 );
                memcpy( target, start, end - start );
                target[end - start] = '\0';
                score.scoreA = atoi(target);
            }
        }
        else if (strstr(buffer, "scoreB") != NULL)
        {
            start = strstr(buffer, "<scoreB>");
            start += strlen("<scoreB>");
            end = strstr(start, "</scoreB>");
            if (end != NULL)
            {
                target = ( char * )malloc( end - start + 1 );
                memcpy( target, start, end - start );
                target[end - start] = '\0';
                score.scoreB = atoi(target);
            }
        }
    }
    free(buffer);
    if (fp != NULL)
    {
        fclose(fp);
    }
    play(MAN_VS_MAN);
}

#if !__cplusplus
void parseDBSave(char *name)
{
    // Initializing Values by default
    paddle.a = conf.boardHeight / 2;
    paddle.b = conf.boardHeight / 2;
    ball.x = conf.boardWidth / 2;
    ball.y = conf.boardHeight / 2;
    ball.dx = -1;
    ball.dy = 1;
    user_input = 0;
    counter = 0;
    ball_delay = 10;

    char msg[512];
    snprintf(msg, 512, "SELECT * FROM save WHERE name LIKE '%s'", name);
    if (mysql_query(conn, msg))
    {
        printLog(LOG_ERROR, (char*)"%s\n", mysql_error(conn));
    }
    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL)
    {
        printLog(LOG_ERROR, (char*)"%s\n", mysql_error(conn));
        return;
    }
    int numFields = mysql_num_fields(result);

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < numFields; i++)
        {
            if (i <= 1)
            {
                continue;
            }
            printLog(LOG_DEBUG, "%s \n", row[i] ? row[i] : "NULL");
            switch (i)
            {
            case 2:
                ball.x = atoi(row[i]);
                break;
            case 3:
                ball.y = atoi(row[i]);
                break;
            case 4:
                ball.dx = atoi(row[i]);
                break;
            case 5:
                ball.dy = atoi(row[i]);
                break;
            case 6:
                paddle.a = atoi(row[i]);
                break;
            case 7:
                paddle.b = atoi(row[i]);
                break;
            case 8:
                score.scoreA = atoi(row[i]);
                break;
            case 9:
                score.scoreB = atoi(row[i]);
                break;
            default:
                break;
            }
        }
    }

    play(MAN_VS_MAN);
}
#endif
