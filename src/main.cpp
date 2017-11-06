#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#include "gameboard.h"

#define SECOND 1000000

void life(GameBoard& board1, GameBoard& board2);

int main()
{
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    keypad(stdscr, TRUE);
    mousemask(BUTTON1_CLICKED, NULL);
    mouseinterval(0);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);

    int rows;
    int cols;
    getmaxyx(stdscr, rows, cols);
    endwin();

    GameBoard board1(rows, cols);
    GameBoard board2(rows, cols);

    bool done = false;
    MEVENT event;
    board1.curses_print();

    while (!done)
    {
        int c = getch();
        switch (c)
        {
            case 'q':
                done = true;
                break;
            case KEY_MOUSE:
                getmouse(&event);
                if (event.bstate & BUTTON1_PRESSED)
                {
                    int r = event.y;
                    int c = event.x;
                    board2.toggle(r, c);
                    attron(COLOR_PAIR(1 + board2.at(r, c)));
                    mvaddch(r, c, ' ');
                    attroff(COLOR_PAIR(1 + board2.at(r, c)));
                    refresh();
                }
                break;
            case '1':
                board1.curses_print();
                break;
            case '2':
                board2.curses_print();
                break;
            case 'p':
                life(board1, board2);
                break;
        }
    }

    endwin();

    return 0;
}

void life(GameBoard& board1, GameBoard& board2)
{
    while (1)
    {
        if (getch() == 'p')
            return;
        for (unsigned int r = 0; r < board2.get_rows(); r++)
        {
            for (unsigned int c = 0; c < board2.get_cols(); c++)
            {
                unsigned int n = board2.num_neighbors(r, c);
                if (board2.at(r, c) == 1)
                {
                    if (n < 2)
                        board1.unset(r, c);
                    else if (n < 4)
                        board1.set(r, c);
                    else
                        board1.unset(r, c);
                }
                else if (n == 3)
                    board1.set(r, c);
            }
        }
        board1.curses_print();
        usleep(0.03 * SECOND);
        board2 = board1;
    }
}
