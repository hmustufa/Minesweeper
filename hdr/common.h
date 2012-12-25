/* hdr/common.h
 * Contains defines common to all applications
 */

// Define this #define if you want to enable debug prints
//#define DEBUG_PRINTS

/* Always print PRINT_INFO and PRINT_ERROR */
#define PRINT_INFO(...)  printf( __VA_ARGS__ );
#define PRINT_ERROR(...) printf( "\nERROR: " __VA_ARGS__ "\n" );

#ifdef DEBUG_PRINTS
#define DEBUG_INFO(...)  printf( "DEBUG: " __VA_ARGS__ );
#else
#define DEBUG_INFO(...) 
#endif

typedef enum
{
    TOP = 0,
    BOTTOM,
    LEFT,
    RIGHT,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    NUM_NEIGHBORS
} neighbor_directions;
