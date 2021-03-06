#include <fstream>
#include <ncurses.h>

#include "gameboard.h"
#include "stdio.h"

GameBoard::GameBoard(const unsigned int rows, const unsigned int cols) :
    rows(rows), cols(cols), size(rows * cols)
{
    int num_chars = size / 8;
    if (size % 8 != 0) num_chars++;
    grid = new BYTE[num_chars]();
}

GameBoard::GameBoard(std::string filename, const unsigned int rows,
        const unsigned int cols) :
    rows(rows), cols(cols), size(rows * cols)
{
    std::ifstream fin;
    // TODO: Exception handling here
    fin.open(filename);

    // Read start position of the grid from the file
    int start_row;
    int start_col;
    int num_rows;
    int num_cols;
    fin >> start_row >> start_col >> num_rows >> num_cols;

    // Eat the newline
    fin.ignore(100, '\n');

    // Set up the grid
    int num_chars = size / 8;
    if (size % 8 != 0) num_chars++;
    grid = new BYTE[num_chars]();

    std::string line;
    for (int r = 0; r < num_rows; r++)
    {
        getline(fin, line);
        for (int c = 0; c < num_cols; c++)
        {
            if (line[c] == 'X')
                set(r + start_row, c + start_col);
        }
    }
    fin.close();
}

GameBoard::GameBoard(const GameBoard& other) :
    rows(other.rows), cols(other.cols), size(other.size)
{
    int num_chars = size / 8;
    if (size % 8 != 0) num_chars++;
    grid = new BYTE[num_chars];
    for (int i = 0; i < num_chars; i++)
        grid[i] = other.grid[i];
}

const GameBoard& GameBoard::operator=(GameBoard rhs)
{
    if (this != &rhs)
    {
        size = rhs.size;
        rows = rhs.rows;
        cols = rhs.cols;
        BYTE* temp = grid;
        grid = rhs.grid;
        rhs.grid = temp;
    }
    return *this;
}

GameBoard::~GameBoard()
{
    delete [] grid;
    grid = nullptr;
    size = 0;
    rows = 0;
    cols = 0;
}

void GameBoard::print() const
{
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            printf("%d", at(r, c));
        }
        printf("\n");
    }
}

void GameBoard::curses_print() const
{
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            attron(COLOR_PAIR(at(r, c) + 1));
            mvwaddch(stdscr, r, c, ' ');
        }
    }
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
    refresh();
}

unsigned int GameBoard::num_neighbors(int r, int c) const
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
        if ((newr >= 0) && (newr < static_cast<int>(rows))
         && (newc >= 0) && (newc < static_cast<int>(cols))
         && at(newr, newc) == 1)
            num++;
    }
    return num;
}

void GameBoard::set(unsigned int r, unsigned int c)
{
    unsigned int i = r*cols + c;
    if ((i >= 0) && (i < size))
    {
        unsigned int index = i / 8;
        unsigned int offset = i % 8;
        grid[index] |= (1 << offset);
    }
}

void GameBoard::unset(unsigned int r, unsigned int c)
{
    unsigned int i = r*cols + c;
    if ((i >= 0) && (i < size))
    {
        unsigned int index = i / 8;
        unsigned int offset = i % 8;
        grid[index] &= ~(1 << offset);
    }
}

void GameBoard::toggle(unsigned int r, unsigned int c)
{
    unsigned int i = r*cols + c;
    if ((i >= 0) && (i < size))
    {
        unsigned int index = i / 8;
        unsigned int offset = i % 8;
        grid[index] ^= (1 << offset);
    }
}

bool GameBoard::at(unsigned int r, unsigned int c)
{
    return static_cast<const GameBoard&>(*this).at(r, c);
}

bool GameBoard::at(unsigned int r, unsigned int c) const
{
    unsigned int i = r*cols + c;
    if ((i >= 0) && (i < size))
    {
        unsigned int index = i / 8;
        unsigned int offset = i % 8;
        return (grid[index] >> offset) & 0x01;
    }
}
