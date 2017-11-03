#ifndef GAMEBOARD_H
#define GAMEBOARD_H

class GameBoard
{
    public:
        // Default constructor, just so we have it
        GameBoard() {}

        // Constructor taking number fo rows and columns
        GameBoard(const unsigned int rows, const unsigned int cols);

        // Destructor
        ~GameBoard();

        // Printer
        void print() const;

        // Mutators
        void set(unsigned int r, unsigned int c);
        void unset(unsigned int r, unsigned int c);
        void toggle(unsigned int r, unsigned int c);

        // Accessors
        bool at(unsigned int r, unsigned int c);
        bool at(unsigned int r, unsigned int c) const;

    private:
        // For convenience
        typedef unsigned char BYTE;

        // Dimensions
        unsigned int rows;
        unsigned int cols;

        // Size of the underlying data structure
        unsigned int size;

        // Underlying array representing the data
        BYTE* grid;
};

#endif
