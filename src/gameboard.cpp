#include "gameboard.h"
#include "stdio.h"

GameBoard::GameBoard(const unsigned int rows, const unsigned int cols) :
    rows(rows), cols(cols), size(rows * cols)
{
    int num_chars = size / 8;
    if (size % 8 != 0) num_chars++;
    grid = new BYTE[num_chars]();
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
        grid[index] &= (~0 ^ (1 << i));
    }
}

bool GameBoard::at(unsigned int r, unsigned int c)
{
    unsigned int i = r*cols + c;
    if ((i >= 0) && (i < size))
    {
        unsigned int index = i / 8;
        unsigned int offset = i % 8;
        return (grid[index] >> offset) & 0x01;
    }
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
