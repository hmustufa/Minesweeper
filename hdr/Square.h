/* hdr/Square.h
 * 
 * Struct containing all information for a single square
 * on the board
 *
 */

/******************************************************
                        INCLUDES
*******************************************************/
#include "common.h"

/******************************************************
                   TYPEDEFS AND ENUMS
*******************************************************/
typedef enum
{
    UNKNOWN = 0,
    REVEALED,
    MARKED
} square_state;

/******************************************************
                    CLASS DEFINITION
*******************************************************/
struct Square
{
 private:
    square_state state;
    bool mine;
    int num_neighbor_mines;
    Square* neighbors[NUM_NEIGHBORS];
#ifdef DEBUG_PRINTS
    int my_row, my_column;
#endif    
    int reveal_recurse(int num_revealed);

 public:
    // Constructions
    Square(); 
    
    // Destructor
    ~Square();
    
    // Methods
    bool is_mine();
    void set_mine();
    
    square_state get_state();
    
    void set_top( Square* s );
    void set_bottom( Square* s );
    void set_left( Square* s );
    void set_right( Square* s );
    void set_top_left( Square* s );
    void set_top_right( Square* s );
    void set_bottom_left( Square* s );
    void set_bottom_right( Square* s );

    int get_neighbor_mines();
    void calc_neighbor_mines();
    
    void mark();
    int reveal();

#ifdef DEBUG_PRINTS
    void set_row(int _row);
    void set_column(int _col);
#endif
}; 