/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();
    
    //Input array
    char input[12];

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");

        fgets(input, 12, stdin);
        int tile = atoi(input);
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int value  = (d*d) - 1;

    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if((d % 2 == 0) && (value == 2))
                board[i][j] = 1;
            else if((d % 2 == 0) && (value == 1))
                board[i][j] = 2;
            else
                board[i][j] = value;
             value -= 1;
        }
    }
    
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == 0)
                printf(" _ ");
            else if(board[i][j] < 10)
                printf(" %d ", board[i][j]);
            else
                printf("%d ", board[i][j]);
        }

        printf("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    //Deal with too big
    if( (tile > ((d*d) - 1)) || (tile < 0))
        return false;

    int tilex;
    int tiley;
    //Find tile coordinates
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if((board[i][j]) == tile)
            {
                tilex = i;
                tiley = j;
            }
        }
        
    }
    int zerox;
    int zeroy;
    //Find zero coordinates
    for(int x = 0; x < d; x++)
    {
        for(int y = 0; y < d; y++)
        {
            if((board[x][y]) == 0)
            {
                zerox = x;
                zeroy = y;
            }
        }
    }

    bool movable = false;

    //Check if points are adjecent
    if( (abs(zerox-tilex) == 1) && (zeroy == tiley) )
       movable = true; 
        
    
    if( (abs(zeroy-tiley) == 1) && (zerox == tilex) )
       movable = true; 
        
    
    if(movable){
        board[zerox][zeroy] = tile;
        board[tilex][tiley] = 0;
    }

    return movable;;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int winner = 1;
    
    
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if((board[i][j]) != winner)
                return false;
            winner++;
        }

    }

    return true;
    
}
