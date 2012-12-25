/* hdr/Board.h
 * 
 * Struct containing all board-level information
 *
 */

/******************************************************
                        INCLUDES
*******************************************************/
#include <string>

#include "Square.h"

/******************************************************
                    CLASS DEFINITION
*******************************************************/
struct Board
{
 private:
    int rows, columns, mines;
    int squares_revealed;
    struct Square* squares;
    bool game_over;
    bool game_won;

 public:
    // Constructions
    Board( int _rows, int _columns, int _mines ); 
    
    // Destructor
    ~Board();
    
    // Methods
    void print_board();
    void populate_neighbors();
    bool parse_input(std::string user_input);
    
    int get_rows();
    int get_columns();
    bool make_move(int move_row, int move_col, bool mark_square);
    
    bool did_we_win();
}; 