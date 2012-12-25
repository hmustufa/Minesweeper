/* src/Board.cc
 * 
 * Implementation of struct containing all board-level
 * information
 *
 */

/******************************************************
                        INCLUDES
*******************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "Board.h"

/******************************************************
              LOCAL FUNCTIONS DEFINITION
*******************************************************/
bool string_valid(std::string user_input);
bool string_valid_number(std::string user_input);

/******************************************************
             CLASS FUNCTION IMPLEMENTATION
*******************************************************/

/* Constructor
 * 
 * Sets up a board
 *
 * Inputs:  _rows    - number of rows in board
 *          _columns - number of columns in board
 *          _mines   - number of mines in board
 * Outputs: (none)
 * Returns: Board struct
 */
Board::Board( int _rows, int _columns, int _mines )
{
    int i, random;
    
    rows = _rows;
    columns = _columns,
    mines = _mines;
    
    /* Set up array of squares */
    squares = new Square[rows*columns];
    
    DEBUG_INFO("New board.  Rows %d, Columns %d, mines %d\n", 
               rows, columns, mines);
    
#ifdef DEBUG_PRINTS
    for (i = 0; i < (rows*columns); i++)
    {
        squares[i].set_column(i%columns+1);
        squares[i].set_row( ( (i-(i%columns))/columns) + 1 );
    }
#endif

    i = 0;
    while ( i < mines )
    {
        /* Randomly assigned mines */
        random = rand() % (rows*columns);
        if ( !squares[random].is_mine() )
        {
            squares[random].set_mine();
            i++;
        }
    }
    
    /* Let squares know who their neighbors are */
    populate_neighbors();
    
    /* Initial values */
    game_over = false;
    game_won =  false;
    squares_revealed = 0;
    
    return;
}

/* Destructor
 * 
 * Destroys a board
 *
 * Inputs:  (none)
 * Outputs: (none)
 * Returns: void
 */
Board::~Board()
{
    delete squares;
    return;
}

/* print_board
 * 
 * Prints board
 * If game is over, incorrectly marked squares 
 * will be displayed
 *
 * Inputs:  (none)
 * Outputs: (none)
 * Returns: void
 */
void Board::print_board()
{
    int i, j;
    
    PRINT_INFO("    ");
    for (i = 0; i < columns; i++)
    {
        PRINT_INFO("%2d ", i+1);
    }
    PRINT_INFO("\n\n");
    
    for (i = 0; i < rows; i++)
    {
        PRINT_INFO("%2d   ", i+1);
        for (j = 0; j < columns; j++)
        {
            switch( squares[i*columns + j].get_state() )
            {
            case UNKNOWN:
                PRINT_INFO("*");
                break;
            case REVEALED:
                if ( squares[i*columns + j].is_mine() )
                {
                    PRINT_INFO("!");
                }
                else
                {
                    PRINT_INFO( "%d", 
                                squares[i*columns + j].get_neighbor_mines() 
                              );
                }
                break;
            case MARKED:
                if (game_over)
                {
                    if (squares[i*columns + j].is_mine() )
                    {
                        PRINT_INFO("m");
                    }
                    else
                    {
                        PRINT_INFO("x");
                    }
                }
                else
                {
                    PRINT_INFO("m");
                }
                break;
            }
            PRINT_INFO("  ");
        }
        PRINT_INFO("\n");
    }
}

/* populate_neighbors
 * 
 * Let all the squares know who their neighbors are
 *
 * Inputs:  (none)
 * Outputs: (none)
 * Returns: void
 */
void Board::populate_neighbors()
{
    int i, j;
    bool has_left, has_right, has_top, has_bottom;
    
    DEBUG_INFO("Populate num neighbors\n");
    
    for (i = 0; i < rows*columns; i++)
    {
        has_left = false;
        has_right = false;
        has_top = false;
        has_bottom = false;
        
        if ( (i-columns) > 0)
        {
            has_top = true;
        }
        
        if ( (i+columns) < (rows*columns) )
        {
            has_bottom = true;
        }
        
        if ( (i%columns) != 0)
        {
            has_left = true;
        }
        
        if ( ( (i+1)%columns ) != 0)
        {
            has_right = true;
        }
        
        /* Iterate through all neighbors */
        for (j = 0; j < NUM_NEIGHBORS; j++)
        {
            switch(j)
            {
            case TOP:
                if (has_top)
                {
                    squares[i].set_top(&squares[i-columns]);
                }
                break;
            
            case BOTTOM:
                if (has_bottom)
                {
                    squares[i].set_bottom(&squares[i+columns]);
                }
                break;
            
            case LEFT:
                if (has_left)
                {
                    squares[i].set_left(&squares[i-1]);
                }
                break;
            
            case RIGHT:
                if (has_right)
                {
                    squares[i].set_right(&squares[i+1]);
                }
                break;
            
            case TOP_LEFT:
                if (has_top && has_left)
                {
                    squares[i].set_top_left(&squares[i-columns-1]);
                }
                break;
            
            case TOP_RIGHT:
                if (has_top && has_right)
                {
                    squares[i].set_top_right(&squares[i-columns+1]);
                }
                break;
            
            case BOTTOM_LEFT:
                if (has_bottom && has_left)
                {
                    squares[i].set_bottom_left(&squares[i+columns-1]);
                }
                break;
            
            case BOTTOM_RIGHT:
                if (has_bottom && has_right)
                {
                    squares[i].set_bottom_right(&squares[i+columns+1]);
                }
                break;
            
            case NUM_NEIGHBORS:
                /* Can't happen but makes the compiler happy :) */
                break;
            }
        }
        squares[i].calc_neighbor_mines();
    }
}

/* parse_input
 * 
 * Parses user input and makes correctly formatted moves 
 * In the case of incorrect format, any moves after the incorrect
 * format will be discarded.
 *
 * Inputs:  user_input - string of moves
 * Outputs: (none)
 * Returns: true if game is over
 *          false otherwise
 */
bool Board::parse_input(std::string user_input)
{
    size_t comma_index = -1, openp_index = -1, closep_index = -1;
    char c;
    bool mark_square;
    std::string temp;
    int move_row, move_col;
    bool move_success;
    
    /* First make sure string has valid characters */
    if ( !string_valid(user_input) )
    {
        PRINT_ERROR("Invalid input!\n");
        return false;
    }
    
    DEBUG_INFO("String is valid!\n");
    
    while( 1 )
    {
        mark_square = false;
        
        openp_index = user_input.find("(", openp_index + 1);
        if (openp_index == (size_t) -1)
        {
            break;
        }
        
        /* Is this marking a spot or making a move? */
        if (openp_index > 0)
        {
            c = user_input.at(openp_index - 1);
            if ( (c == 'm') ||
                 (c == 'M')
               )
            {
                DEBUG_INFO("Marking square\n");
                mark_square = true;
            }
#ifdef DEBUG_PRINTS
            else
            {
                DEBUG_INFO("Making move\n");
            }
#endif            
        }
        
        /* Found open parenthesis - Find comma to get row */
        comma_index = user_input.find(",", openp_index);
        if (comma_index == (size_t) -1)
        {
            PRINT_INFO("Move invalid (can't find comma)!\n");
            break;
        }
        temp = user_input.substr(openp_index + 1, 
                                 (comma_index - openp_index - 1)
                                );
        /* Make sure it's just a number in between */
        if ( !string_valid_number(temp) )
        {
            PRINT_INFO("Move invalid (row not a number)!\n");
            break;
        }
        
        move_row = atoi(temp.c_str() ) - 1;
        DEBUG_INFO("Move row %d\n", move_row + 1);
        
        /* Next find close parenthesis */
        closep_index = user_input.find(")", comma_index);
        if (closep_index == (size_t) -1)
        {
            PRINT_INFO("Move invalid (cannot find close parenthesis)!\n");
            break;
        }
        temp = user_input.substr(comma_index + 1,
                                 (closep_index - comma_index - 1)
                                );
        /* Make sure it's just a number in between */
        if ( !string_valid_number(temp) )
        {
            PRINT_INFO("Move invalid (column not a number)!\n");
            break;
        }
        move_col = atoi(temp.c_str()) - 1;
        DEBUG_INFO("Move col %d\n", move_col + 1);
        
        move_success = this->make_move(move_row, move_col, mark_square);
        if (!move_success)
        {
            DEBUG_INFO("Game over, didn't win\n");
            game_over = true;
            game_won = false;
            return game_over;
        }
        
    }
    
    /* Made all moves - have we revealed all squares? */
    if ( squares_revealed == ( (rows*columns) - mines ) )
    {
        DEBUG_INFO("Game over, won\n");
        game_won = true;
        game_over = true;
    }

    DEBUG_INFO("Game not over yet ... \n");
    return game_over;
}

/* make_move
 * 
 * Makes the specified move
 *
 * Inputs:  move_row - Row is square affected
 *          move_col - Column of square affected
 *          mark_square - true if just marking square
 *                        false if making a move
 * Outputs: (none)
 * Returns: true if moves successfully made
 *          false if selected a mine
 */
bool Board::make_move(int move_row, int move_col, bool mark_square)
{
    if ( mark_square )
    {
        PRINT_INFO("Marking (%d,%d)\n", move_row + 1, move_col + 1);
        squares[move_row*columns + move_col].mark();
    }
    else
    {
        PRINT_INFO("Making a move on (%d,%d)\n", move_row + 1, move_col + 1);
        squares_revealed += squares[move_row*columns + move_col].reveal();
        if (squares[move_row*columns + move_col].is_mine())
        {
            DEBUG_INFO("Made a move on a mine!\n");
            return false;
        }
        
    }
    return true;
}

/* did_we_win
 * 
 * Did we win? :)
 *
 * Inputs:  (none)
 * Outputs: (none)
 * Returns: true if we won the game
 *          false otherwise
 */
bool Board::did_we_win()
{
    return game_won;
}

/******************************************************
             LOCAL FUNCTION IMPLEMENTATIONS
*******************************************************/

/* string_valid
 * 
 * Checks if all characters are valid 
 * (Can only have white space, open parenthesis, 
 * close_parenthesis, commas, M/m, and numbers
 *
 * Inputs:  user_input - String from user
 * Outputs: (none)
 * Returns: true if string is valid
 *          false otherwise
 */
bool string_valid(std::string user_input)
{
    size_t i;
    char c;
    
    for (i = 0; i < user_input.length(); i++)
    {
        c = user_input.at(i);
        
        if ( ( c >  ' ' ) &&
             ( c != '(' ) &&
             ( c != ')' ) &&
             ( c != ',' ) &&
             ( c != 'M' ) &&
             ( c != 'm' ) &&
             ( ( c <  '0' ) ||
               ( c >  '9' )
             )
           )
        {
            return false;
        }
    }
    
    return true;

}

/* string_valid_number
 * 
 * Checks if string contains just numbers and white
 * space
 *
 * Inputs:  user_input - parsed string
 * Outputs: (none)
 * Returns: true if string is valid
 *          false otherwise
 */
bool string_valid_number(std::string user_input)
{
    size_t i;
    char c;
    
    for (i = 0; i < user_input.length(); i++)
    {
        c = user_input.at(i);
        
        if ( ( c > ' ' ) &&
             ( ( c < '0' ) ||
               ( c > '9' )
             )
           )
        {
            return false;
        }
    }
    
    return true;

}