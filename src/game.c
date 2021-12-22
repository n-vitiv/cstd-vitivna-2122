#include "../include/game.h"
#include "../include/logger.h"

#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

#define FRAMETIME 80000

int user_input;
paddleData paddle;
ballData ball;
scoreCount score;
configure conf;
int counter;
int ball_delay;

void menu()
{
    score.scoreA = 0;
    score.scoreB = 0;
    system("clear");
    int mode = 1;
    printf("======================================================================================\n");
    printf("=                                  Ping Pong game                                    =\n");
    printf("=                                                                                    =\n");
    printf("======================================================================================\n");
    printf("= Game modes                                                                         =\n");
    printf("= 1. Man vs Man                                                                      =\n");
    printf("= 2. Man vs AI                                                                       =\n");
    printf("= 3. AI vs AI                                                                        =\n");
    printf("======================================================================================\n");
    printf("= 4. Quit game                                                                       =\n");
    printf("======================================================================================\n");
    printf("= Enter your choice (1-4): ");
    scanf("%d", &mode);
    modeChoice(mode);
}

void modeChoice(int mode)
{
    switch (mode)
    {
    case 1:
        system("clear");
        printLog(LOG_DEBUG, "mode: man vs man");
        manVsMan();
        break;
    
    case 2:
        system("clear");
        printLog(LOG_DEBUG, "mode: man vs ai");
        manVsAI();
        break;

    case 3:
        system("clear");
        printLog(LOG_DEBUG, "mode: ai vs ai");
        AIvsAI();
        break;

    case 4:
        printf("Leaving game.\n");
        sleep(3);
        system("clear");
        break;

    default:
        printf("You entered bad mode. Please try again after 5 seconds.\n");
        sleep(5);
        system("clear");
        menu();
        break;
    }
}

void manVsMan()
{
    WINDOW *w = initscr();
    noecho();
    curs_set(FALSE);
    cbreak();
    nodelay(w, TRUE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    while (score.scoreA < 10 && score.scoreB < 10 && user_input != 'q')
    {
        initial_print();
        draw_scoreline();
        int loop;
        while (user_input != 'q')
        {
            user_input = getch();
            loop = game_loop();
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

    printf("========================== WINNER ================================\n");
    if (score.scoreA >= 10)
    {
        printf("========================= PLAYER A ===============================\n");
    }
    else
    {
        printf("========================= PLAYER B ===============================\n");
    }
    printf("==================================================================\n");
    sleep(5);
    menu();
}

void manVsAI()
{
    menu();
}

void AIvsAI()
{
    menu();
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

int game_loop()
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
    char *a = malloc(10);
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
    conf.boardHeight = atoi(getValue("board_height", &cParser));
    if (conf.boardHeight <= 0)
    {
        conf.boardHeight = 24;
    }

    conf.boardWidth = atoi(getValue("board_width", &cParser));
    if (conf.boardWidth <= 0)
    {
        conf.boardWidth = 80;
    }

    conf.paddleLength = atoi(getValue("tool_size", &cParser));
    if (conf.paddleLength <= 0)
    {
        conf.paddleLength = 5;
    }
}
