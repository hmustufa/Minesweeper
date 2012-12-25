/* src/Square.cc
 * 
 * Implementation of struct containing all information for 
 * a single square on the board
 *
 */

/******************************************************
                        INCLUDES
*******************************************************/

#include <stdio.h>

#include "Square.h"

/******************************************************
             CLASS FUNCTION IMPLEMENTATION
*******************************************************/

/* Constructor
 * 
 * Sets up a square
 *
 * Inputs:  (none)
 * Outputs: (none)
 * Returns: Square struct
 */
Square::Square( )
{
    int i;
 #ifdef DEBUG_PRINTS
    static int num_squares = 1;
 #endif 
    
    DEBUG_INFO("New square %d\n", num_squares++);
    
    mine = false;
    state = UNKNOWN;
    num_neighbor_mines = 0;
    
    for (i = 0; i < NUM_NEIGHBORS; i++)
    {
        neighbors[i] = NULL;
    }

#ifdef DEBUG_PRINTS
    my_column = 0;
    my_row = 0;
#endif

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
Square::~Square()
{
    return;
}

/* is_mine
 * 
 * Returns whether this square is a mine or none
 *
 * Inputs:  (none)
 * Outputs: (none)
 * Returns: true if square is a mine
 *          false otherwise
 */
bool Square::is_mine()
{
    return mine;
}

/* set_mine
 * 
 * Sets this square as a mine
 *
 * Inputs:  (none)
 * Outputs: (none)
 * Returns: void
 */
void Square::set_mine()
{
 #ifdef DEBUG_PRINTS
    static int num_mines = 1;
 #endif
 
    mine = true;
    DEBUG_INFO("%d.%d) Num mine %d\n", my_row, my_column,
                num_mines++);
}

/* get_state
 * 
 * Returns state of the board (see enum in hdr/Square.h)
 *
 * Inputs:  (none)
 * Outputs: (none)
 * Returns: State of square
 */
square_state Square::get_state()
{
    return state;
}

/* set_top
 * 
 * Sets the top neighbor
 *
 * Inputs:  s - Pointer to square on top
 * Outputs: (none)
 * Returns: void
 */
void Square::set_top( Square* s )
{
    neighbors[TOP] = s;
}

/* set_bottom
 * 
 * Sets the bottom neighbor
 *
 * Inputs:  s - Pointer to square on bottom
 * Outputs: (none)
 * Returns: void
 */
void Square::set_bottom( Square* s )
{
    neighbors[BOTTOM] = s;
}

/* set_left
 * 
 * Sets the left neighbor
 *
 * Inputs:  s - Pointer to square to left
 * Outputs: (none)
 * Returns: void
 */
void Square::set_left( Square* s )
{
    neighbors[LEFT] = s;
}

/* set_right
 * 
 * Sets the right neighbor
 *
 * Inputs:  s - Pointer to square to right
 * Outputs: (none)
 * Returns: void
 */
void Square::set_right( Square* s )
{
    neighbors[RIGHT] = s;
}

/* set_top_left
 * 
 * Sets the top-left neighbor
 *
 * Inputs:  s - Pointer to square on top-left
 * Outputs: (none)
 * Returns: void
 */
void Square::set_top_left( Square* s )
{
    neighbors[TOP_LEFT] = s;
}

/* set_top_right
 * 
 * Sets the top-right neighbor
 *
 * Inputs:  s - Pointer to square on top-right
 * Outputs: (none)
 * Returns: void
 */
void Square::set_top_right( Square* s )
{
    neighbors[TOP_RIGHT] = s;
}

/* set_bottom_left
 * 
 * Sets the bottom-left neighbor
 *
 * Inputs:  s - Pointer to square on bottom-left
 * Outputs: (none)
 * Returns: void
 */
void Square::set_bottom_left( Square* s )
{
    neighbors[BOTTOM_LEFT] = s;
}

/* set_bottom_right
 * 
 * Sets the bottom-right neighbor
 *
 * Inputs:  s - Pointer to square on bottom-right
 * Outputs: (none)
 * Returns: void
 */
void Square::set_bottom_right( Square* s )
{
    neighbors[BOTTOM_RIGHT] = s;
}

/* get_neighbor_mines
 * 
 * Returns how many neighbors are mines
 *
 * Inputs:  (none)
 * Outputs: (none)
 * Returns: number of neighbor mines
 */
int Square::get_neighbor_mines()
{
    return num_neighbor_mines;
}

/* calc_neighbor_mines
 * 
 * Calculates how many neighbors are mines.
 * Sets num_neighbor_mines within struct
 *
 * Inputs:  (none)
 * Outputs: (none)
 * Returns: void
 */
void Square::calc_neighbor_mines()
{
    int i;
    
    num_neighbor_mines = 0;
    
    if (mine)
    {
        DEBUG_INFO("%d.%d) is a mine\n",
                   my_row, my_column
                  );
        return;
    }
    
    for (i = 0; i < NUM_NEIGHBORS; i++)
    {
        if ( (neighbors[i] != NULL) &&
             (neighbors[i]->is_mine())
           )
        {
            num_neighbor_mines++;
        }
    }
    DEBUG_INFO("%d.%d) Neighbors %d\n",
               my_row, my_column,
               num_neighbor_mines);
    return;
}

/* mark
 * 
 * Sets the square as MARKED
 *
 * Inputs:  (none)
 * Outputs: (none)
 * Returns: void
 */
void Square::mark()
{
    state = MARKED;
}

/* reveal
 * 
 * Reveals the square
 * Calls private recursive function
 *
 * Inputs:  (none)
 * Outputs: (none)
 * Returns: Number of revealed squares
 */
int Square::reveal()
{
    return this->reveal_recurse(0);
}

/* reveal_recurse
 * 
 * Private function for recursively revealing
 * squares when there are 0 neighboring mines
 *
 * Inputs:  num_revealed - number of squares already revealed
 * Outputs: (none)
 * Returns: number of squares revealed
 */
int Square::reveal_recurse(int num_revealed)
{
    int i;
    
    /* Just return if already revealed */
    if (state == REVEALED)
    {
        DEBUG_INFO("%d.%d) Already revealed!\n",
                    my_row, my_column
                  );
        return 0;
    }
    
    /* Change state */
    state = REVEALED;
    
    /* It's a mine!  Oh no! */
    if (mine)
    {
        /* Oh no! */
        DEBUG_INFO("%d.%d) revealed a mine!\n",
                    my_row, my_column
                  );
        return 0;
    }
    
    /* I have neighbors ... return just myself */
    if (num_neighbor_mines != 0)
    {
        DEBUG_INFO("%d.%d) has %d neighbor mines\n", 
                    my_row, my_column,
                    num_neighbor_mines
                  );
        return 1;
    }
    
    DEBUG_INFO("%d.%d) 0 neighbor mines\n",
                my_row, my_column
              );
    num_revealed = 1;
    /* There are no neighboring mines */
    /* Recurively reveal the rest */
    for (i = 0; i < NUM_NEIGHBORS; i++)
    {
        if ( neighbors[i] != NULL )
        {        
            num_revealed += neighbors[i]->reveal();
        }
    }

    DEBUG_INFO("%d.%d) revealed %d\n", 
                my_row, my_column,
                num_revealed
              );
    return num_revealed;    
}

#ifdef DEBUG_PRINTS
/* set_rows
 * 
 * Sets which row of the board this square is in
 * This is only used for debugging purposes
 *
 * Inputs:  _row - the row of this square
 * Outputs: (none)
 * Returns: void
 */
void Square::set_row(int _row)
{
    my_row = _row;
}

/* set_column
 * 
 * Sets which row of the board this square is in
 * This is only used for debugging purposes
 *
 * Inputs:  _col - the column of this square
 * Outputs: (none)
 * Returns: void
 */
void Square::set_column(int _col)
{
    my_column = _col;
}
#endif