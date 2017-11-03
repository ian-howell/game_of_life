#include <curses.h>
#include <stdio.h>
#include <unistd.h>

#define SECOND 1000000

void print_grid(char** grid, int rows, int cols);
void gridcpy(char** grid1, char** grid2, int rows, int cols);
void life(char** grid1, char** grid2, int rows, int cols, int length);
int num_neighbors(char** grid, int r, int c, int maxr, int maxc);

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

    char** grid1 = new char*[rows];
    char** grid2 = new char*[rows];
    for (int r = 0; r < rows; r++)
    {
        grid1[r] = new char[cols];
        grid2[r] = new char[cols];
        for (int c = 0; c < cols; c++)
        {
            grid1[r][c] = '.';
            grid2[r][c] = '.';
        }
    }

    bool done = false;
    MEVENT event;
    print_grid(grid1, rows, cols);
    gridcpy(grid1, grid2, rows, cols);
    refresh();
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
                    grid2[event.y][event.x] = 'X';
                    attron(COLOR_PAIR(2));
                    mvaddch(event.y, event.x, ' ');
                    attroff(COLOR_PAIR(2));
                    refresh();
                }
                break;
            case '1':
                print_grid(grid1, rows, cols);
                refresh();
                break;
            case '2':
                print_grid(grid2, rows, cols);
                refresh();
                break;
            case 'p':
                life(grid1, grid2, rows, cols, 100);
                break;
        }
    }

    endwin();

    printf("%d, %d\n", rows, cols);

    for (int i = 0; i < rows; i++)
    {
        delete [] grid1[i];
        delete [] grid2[i];
    }
    delete [] grid1;
    delete [] grid2;

    return 0;
}

void print_grid(char** grid, int rows, int cols)
{
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
        {
            if (grid[r][c] == '.')
                attron(COLOR_PAIR(1));
            else
                attron(COLOR_PAIR(2));
            mvwaddch(stdscr, r, c, ' ');
        }
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
    refresh();
}

void gridcpy(char** grid1, char** grid2, int rows, int cols)
{
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            grid1[r][c] = grid2[r][c];
}

void life(char** grid1, char** grid2, int rows, int cols, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (getch() == 'p')
            return;
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                int n = num_neighbors(grid2, r, c, rows, cols);
                if (grid2[r][c] == 'X')
                {
                    if (n < 2)
                        grid1[r][c] = '.';
                    else if (n < 4)
                        grid1[r][c] = 'X';
                    else
                        grid1[r][c] = '.';
                }
                else if (n == 3)
                    grid1[r][c] = 'X';
            }
        }

        print_grid(grid1, rows, cols);
        refresh();
        usleep(0.05 * SECOND);
        gridcpy(grid2, grid1, rows, cols);
    }
}

int num_neighbors(char** grid, int r, int c, int maxr, int maxc)
{
  int offset[8][2] = {
    {-1, 0},  // UP
    {+1, 0},  // DOWN
    {0, +1},  // RIGHT
    {0, -1},  // LEFT
    {-1, -1}, // UP-LEFT
    {-1, +1}, // UP-RIGHT
    {+1, +1}, // DOWN-RIGHT
    {+1, -1}, // DOWN-LEFT
  };
    int num = 0;
    for (int i = 0; i < 8; i++)
    {
        int newr = r + offset[i][0];
        int newc = c + offset[i][1];
        if ((newr >= 0) && (newr < maxr) && (newc >= 0) && (newc < maxc) && grid[newr][newc] == 'X')
            num++;
    }
    return num;
}
