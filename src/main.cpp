#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#include "gameboard.h"

#define SECOND 1000000

void life(GameBoard& board);

int main(const int argc, const char** argv)
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

    // Create the starting board
    GameBoard* original_board = nullptr;
    if (argc == 1)
        original_board = new GameBoard(rows, cols);
    else
        original_board = new GameBoard(argv[1], rows, cols);

    // Create the _actual_ board using the original as a blueprint
    GameBoard board(*original_board);
    board.curses_print();

    bool done = false;
    MEVENT event;
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
                    board.toggle(r, c);
                    attron(COLOR_PAIR(1 + board.at(r, c)));
                    mvaddch(r, c, ' ');
                    attroff(COLOR_PAIR(1 + board.at(r, c)));
                    refresh();
                }
                break;
            case 'p':
                life(board);
                break;
            case 'r':
                // Refesh with the original board
                GameBoard tmp(*original_board);
                board = tmp;
                board.curses_print();
                break;
        }
    }

    delete original_board;
    endwin();

    return 0;
}

void life(GameBoard& board)
{
    GameBoard reverse(board);
    while (1)
    {
        if (getch() == 'p')
            return;
        for (unsigned int r = 0; r < board.get_rows(); r++)
        {
            for (unsigned int c = 0; c < board.get_cols(); c++)
            {
                unsigned int n = board.num_neighbors(r, c);
                if (board.at(r, c) == 1)
                {
                    if (n < 2)
                        reverse.unset(r, c);
                    else if (n < 4)
                        reverse.set(r, c);
                    else
                        reverse.unset(r, c);
                }
                else if (n == 3)
                    reverse.set(r, c);
            }
        }
        board = reverse;
        board.curses_print();
        usleep(0.03 * SECOND);
    }
}
