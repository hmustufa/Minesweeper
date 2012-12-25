#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <string>
#include <sstream>

#include "Board.h"

int main ()
{
    bool game_over = false;
    bool selection_valid = false;
    int board_select, rows, cols, mines;
    std::string user_input;
    struct Board *board;
    int temp;
    
    while (!selection_valid)
    {
        PRINT_INFO("Welcome to Minesweeper!  Select a board (Enter number only):\n" \
                   "1) Easy   (9x9,   10 mines)\n" \
                   "2) Medium (16x16, 40 mines)\n" \
                   "3) Hard   (16x30, 99 mines)\n" \
                   "4) Custom ... \n"
                  );
        
        PRINT_INFO("Enter your selection: ");
        std::cin >> user_input;
        board_select = atoi(user_input.c_str());
    
        if ( (board_select < 1) || (board_select > 4) )
        {
            PRINT_ERROR("Invalid menu item!\n");
        }
        else
        {
            selection_valid = true;
        }
    }
    
    switch (board_select)
    {
    case(1):
        rows = 9;
        cols = 9;
        mines = 10;
        
        break;
    case(2):
        rows = 16;
        cols = 16;
        mines = 40;
        
        break;
    case(3):
        rows = 16;
        cols = 30;
        mines = 99;
        
        break;
    case(4):
        selection_valid = false;
        while (!selection_valid)
        {
            PRINT_INFO("Enter number of rows (min 1, max 25): ");
            std::cin >> user_input;
            rows = atoi(user_input.c_str());
            if ( (rows < 1) || (rows > 25) )
            {
                PRINT_ERROR("Invalid number of rows!  Try again!\n");
            }
            else
            {
                selection_valid = true;
            }
        }
        
        selection_valid = false;
        while (!selection_valid)
        {
            PRINT_INFO("Enter number of columns (min 1, max 25): ");
            std::cin >> user_input;
            cols = atoi(user_input.c_str());
            if ( (cols < 1) || (rows > 25) )
            {
                PRINT_ERROR("Invalid number of columns!  Try again!\n");
            }
            else
            {
                selection_valid = true;
            }
        }
        
        selection_valid = false;
        temp = rows*cols;
        while (!selection_valid)
        {
            PRINT_INFO("Enter number of mines (min 1, max %d) ", temp);
            std::cin >> user_input;
            mines = atoi(user_input.c_str());
            if ( (mines < 1) || (mines > temp) )
            {
                PRINT_ERROR("Invalid number of mines!  Try again!\n");
            }
            else
            {
                selection_valid = true;
            }
        }
        break;
    }
    
    PRINT_INFO("Your board is %dx%d and has %d mines.\n", rows, cols, mines);
    
    board = new Board(rows, cols, mines);
    
    PRINT_INFO("\n\nINSTRUCTIONS\n");
    PRINT_INFO("(row,column) makes a move on a spot.  M(row," \
               "column) will mark a spot as a mine\n");
    PRINT_INFO("Moves can also be comma separated if you want " \
               "to make multiple moves at a time\n\n"
              );
  
    while (!game_over)
    {
        board->print_board();
        PRINT_INFO("Move: ");
        std::cin >> user_input;
        game_over = board->parse_input(user_input);
    }
    
    board->print_board();
    if (board->did_we_win())
    {
        PRINT_INFO("Congratulations, you won!  :D\n");
    }
    else
    {
        PRINT_INFO("Sorry, you lost :'(\n");
    }
  
    return 0;
}