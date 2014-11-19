/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

#define blank_space 999

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
void swap(int row, int column, int i, int j);
bool search(int users_move);
bool won(void);
void save(void);
void sort(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
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
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    int total_numbers = d * d; //set total_numbers to the total of the dimension of the board
    for(int i = 0; i < d; i++)
    {   
        for(int j = 0; j < d; j++)
        {
            board[i][j] =  total_numbers;
            total_numbers--;
        }
    }
    
    //this is the number that will represent a blank character
    board[0][0] = blank_space;
    
    //if the board is made up of an odd number of tiles, swap 1 and 2 as per instructions
    if((d * d) % 2 == 0)
    {
        int temp = board[d-1][d-2];
        board[d-1][d-2] = board[d-1][d-1];
        board[d-1][d-1] = temp;
    }
    sort();
   
}
/**
* this function moves the blank space number
* through the array, as it moves to the lower
* right corner of the board all the other 
* numbers move left 1 space.
*/
void sort()
{
    //sort function.
    for(int i = 0; i <= d - 1; i++)
    {
        int first_index = i + 1;
         
        for(int j = 0; j < d - 1; j++)
        {
            int next_index = j + 1;
            
            //swaps the current index of j with the next index of j
            if(board[i][j] > board[i][next_index])
            {
                int temp = board[i][j];
                board[i][j] = board[i][next_index];
                board[i][next_index] = temp;
            }
            
        }//end inner for loop
        
        /**
        compare the last index of j with the
        first index of the next row board[i+1][0]
        */
        if(i != d - 1)
        {
            if(board[i][d - 1] > board[first_index][0])
            {
                int temp2 = board[i][d - 1];
                board[i][d - 1] = board[first_index][0];
                board[first_index][0] = temp2;
            }
        }    
    }//end for loop
    
}//end sort()


/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int i = 0; i < d; i++)
    {   
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == blank_space)
                printf("|__|");
            else
                printf("|%2d|", board[i][j]);
        }
        
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    bool location = search(tile);
    if(location)
        return true;
    
    return false;
}
/**
* searches for the users tile to be moved
* sends the location of the users tile
* to the swap() function.
* if the users tile is not in a location
* adjacent to the blank tile returns false.
*/
bool search(int users_move)
{
    for(int i = 0; i <= d - 1; i++)
    {
        for(int j = 0; j <= d - 1; j++)
        {
            if(board[i][j] == users_move)
            {
                if(board[i+1][j] == blank_space)
                {
                    swap(i, j, i+1, j);
                    return true;
                }
                else if(board[i][j+1] == blank_space)
                {
                    swap(i, j, i, j+1);
                    return true;
                }
                else if(board[i][j-1] == blank_space)
                {
                    swap(i, j, i, j-1);
                    return true;
                }
                else if(board[i-1][j] == blank_space)
                {
                    swap(i,j, i-1, j);
                    return true;
                }
                
            }            
        }
    }
    
    return false;  
}//end function

/**
* swaps the blank space using the location of
* the users tile provided when this function is called
*/
void swap(int row, int column, int i, int j)
{
    //swapping algorithm
    int temp = board[i][j];
    board[i][j] = board[row][column];
    board[row][column] = temp;
}//end function


/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    //itirate over the current board and check if it is in ascending order
    //increment in_order for the amount of numbers in order.
    int in_order = 0;
    for(int i = 0; i < d; i++)
    {
        int ith_index = 0;
        
        for(int j = 0; j < d; j++)
        {
            int next_index;
            if(j == d - 1)
                next_index = j;
            else
                next_index = j + 1;
                
            if(board[i][j] < board[i][next_index])
                in_order += 1;        
        }
        
        if(i != d - 1)
        {
            ith_index = i + 1;
            
            if(board[i][d-1] < board[ith_index][0])
                in_order += 1;
        }
    }
    
    //if in_order is equal to the number of tiles in order the game is won
    if(in_order == (d * d) - 1)
        return true;
        
    return false; //else the game is not yet won so play must continue
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
