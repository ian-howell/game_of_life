#ifndef GAMEBOARD_H
#define GAMEBOARD_H

class GameBoard
{
    public:
        // Default constructor, just so we have it
        GameBoard() {}

        // Constructor taking number fo rows and columns
        GameBoard(const unsigned int rows, const unsigned int cols);

        // Copy constructor
        GameBoard(const GameBoard& other);

        // Equals assignment
        const GameBoard& operator=(GameBoard rhs);

        // Destructor
        ~GameBoard();

        // Printer
        void print() const;

        // ncurses printer. ncurses must be initialized
        void curses_print() const;

        // Return the number of neighbors at location (r, c)
        unsigned int num_neighbors(int r, int c) const;

        // Mutators
        void set_rows(const unsigned int r) { rows = r; }
        void set_cols(const unsigned int c) { cols = c; }
        void set_size(const unsigned int s) { size = s; }

        void set(unsigned int r, unsigned int c);
        void unset(unsigned int r, unsigned int c);
        void toggle(unsigned int r, unsigned int c);

        // Accessors
        bool at(unsigned int r, unsigned int c);
        bool at(unsigned int r, unsigned int c) const;

        unsigned int get_rows() const { return rows; }
        unsigned int get_cols() const { return cols; }
        unsigned int get_size() const { return size; }

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
