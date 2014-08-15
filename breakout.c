//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

//constants of the paddle when initialised
#define PADDLE_Y_AXIS 550
#define PADDLE_X_AXIS 170
#define PADDLE_WIDTH 60
#define PADDLE_HEIGHT 10

//constants of laser dimensions
#define LASER_HEIGHT 20
#define LASER_WIDTH 5
#define laser_vertical_velocity -0.1

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

//globals


// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GRect initLaser(GWindow window, GRect paddle);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
GObject detectLaserCollision(GWindow window, GRect laser);
void run(GWindow window, GRect paddle, GOval ball, GLabel label, GRect laser, bool god_mode);

int main(int argc, string argv[])
{   
    bool god_mode = false;
    //if the user has issued a cmd line arg of "godmode", set god_mode to true.
    if(argc == 2)
    {
        if(strcmp(argv[1], "godmode") == 0)
            god_mode = true;
    } 
       
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);
    
    GRect laser = NULL;

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    
    //wait for user to click before starting the game.
    waitForClick();
    
    run(window, paddle, ball, label, laser, god_mode);
    
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}


/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    //declaration of variables used to set the co-ords for each brick, ie, the first co-ord is 3,30
    int y_axis = 30;
    int x_axis = 3;
    
    string colours[] = {"red", "orange", "yellow", "green", "blue"}; 
       
    for(int i = 0; i < 5; i++)
    {
        x_axis = 3;
        
        for(int j = 0; j < 10; j++)
        {
            //create a new brick at x_axis and y_axis at each iteration of this loop.
            GRect brick = newGRect(x_axis, y_axis, 37, 10);
            
            //set the colour of the row of bricks
            setColor(brick, colours[i]);
            
            setFilled(brick, true);
            
            add(window, brick);
            
            //before exiting the loop, set x_axis (the location on the x_axis of the next brick) 2 pixels away from the previous.
            x_axis += 39;
        }
        
        //after 10 iterations of j, 10 bricks are made, and so the next row is set, determined by y_axis as 20 pixels below.
        y_axis += 20;
    }
}

/**
* Instantiates a laser at the centre of the paddle
* Returns the laser.
*/
GRect initLaser(GWindow window, GRect paddle)
{
    //set up the initial dimensions and position of the laser
    GRect laser = newGRect(getX(paddle) + (PADDLE_WIDTH / 2), getY(paddle) - PADDLE_HEIGHT * 2, LASER_WIDTH, LASER_HEIGHT);
    setColor(laser, "green");
    setFilled(laser, true);
    add(window, laser);
    return laser;
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    //initialise the ball
    GOval ball = newGOval(200, 300, 20, 20);
    setColor(ball, "black");
    setFilled(ball, true);
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    
    //create the paddle and set it to the middle of the window at the bottom.
    GRect paddle = newGRect(PADDLE_X_AXIS, PADDLE_Y_AXIS, PADDLE_WIDTH, PADDLE_HEIGHT);
    
    //set the colour of the paddle to black and then fill it.
    setColor(paddle, "black");
    setFilled(paddle, true);
    
    add(window, paddle);
    
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    //instatiate the glabel
    GLabel label = newGLabel("0");
    setFont(label, "SansSerif-36");
    add(window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
/**
* This function determines whether the laser has collided with an object 
* in the window. It then returns this object.
*/
GObject detectLaserCollision(GWindow window, GRect laser)
{
    GObject object;
    
    double y = getY(laser);
    double x = getX(laser);
    
    //check for a collision at the top of the laser.
    object = getGObjectAt(window, x, y - LASER_HEIGHT);
    if(object != NULL)
        return object;
        
   //no collision
   return NULL;   
}

/**
* This is the main execution of the game.
* This function starts by declaring variables needed to determine the state of play.
* When program control reaches the while() loop, the program will continue to run until there are no more lives or bricks.
* The while loop handles the movement of the ball, the movement of the paddle (with a cheat mode implemented) and the movement of a laser.
* The while loop also updates the scoreboard, keeps track of how many lives are left, and handles the removal of the bricks and laser when destroyed.
*/
void run(GWindow window, GRect paddle, GOval ball, GLabel label, GRect laser, bool god_mode)
{
    // A value used to determine whether a laser is currently on the screen, only one laser can be fired at a time.
    // If this is set to false, the user can click the left mouse button to fire a laser.
    bool user_clicked_laser = false;

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    //initial velocity of the ball
    double velocity = 0.05;
    double vertical_velocity = 0.05;
    
    //set the label at the begginning of the game
    updateScoreboard(window, label, points);
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        //move the ball 
        move(ball, velocity, vertical_velocity);
        
        //******GOD MODE IMPLEMENTATION*******
        //when god_mode is true, the computer ignores the users mouse movements and moves the paddle matching the x axis of the ball.
        if(god_mode)
            setLocation(paddle, getX(ball), PADDLE_Y_AXIS);
        else
        {   
            //event handling for paddle, operated by user moving the cursor
            GEvent evt = getNextEvent(MOUSE_EVENT); 
                
            //if an event occurs
            if(evt != NULL) 
            {
                //if the event is a mouse moved event
                if(getEventType(evt) == MOUSE_MOVED)
                {
                    double x = getX(evt) - (PADDLE_WIDTH / 2); 
                    setLocation(paddle, x, PADDLE_Y_AXIS);
                }
                //if the event is a mouse click, the paddle "shoots" a laser towards the bricks.
                if(getEventType(evt) == MOUSE_CLICKED && user_clicked_laser != true)
                {
                    user_clicked_laser = true;
                    laser = initLaser(window, paddle);
                }
            }//end event handling for mouse and paddle
        }
        
        //if the user has fired a laser:
        //move that laser towards the bricks,
        //if it hits a brick remove both objects and update the scoreboard,
        //if it misses all bricks allow another laser to be fired.
        if(user_clicked_laser)
        {
            //move the laser upwards
            move(laser, 0, laser_vertical_velocity);
            
            //store the value of collision checking for the laser.
            GObject laser_collision = detectLaserCollision(window, laser);
        
            //if the laser has collided with a brick, remove both objects and add a point to the score
            if(laser_collision != NULL)
            {
                if(strcmp(getType(laser_collision), "GRect") == 0)
                {
                    removeGWindow(window, laser);
                    removeGWindow(window, laser_collision);
                    
                    bricks--;
                    points++;
                    
                    updateScoreboard(window, label, points);
                    
                    user_clicked_laser = false;
                }
            }
            
            //if the laser misses all the bricks remove it and allow another laser to be fired.
            if(getY(laser) <= 0)
            {
                removeGWindow(window, laser);
                user_clicked_laser = false;
            }
        }
        
        //store the value of any collision the ball makes in "object", to be used in the balls movement below.
        GObject object = detectCollision(window, ball);
        
        //if there was no collision, ie the ball bounced off of a wall, change the velocity accordingly
        if(object == NULL)
        {
            //bounce the ball off the right hand edge of the window
            if(getX(ball) + getWidth(ball) >= WIDTH)
                velocity = -velocity;
            
            //bounce the ball off of the left hand edge of the window     
            else if(getX(ball) <= 0)
                velocity = -velocity;
            
            //bounce the ball off the top of the window
            else if(getY(ball) <= 0)
                vertical_velocity = -vertical_velocity;
            
            //if the ball has moved passed the paddle reset the ball and wait for the user to click before continuing
            else if(getY(ball) + getHeight(ball) >= getHeight(window))
            {
                setLocation(ball, 200, 300);
               
                lives--; //user loses a life
                waitForClick();
            }
        }
        else
        {
            //bounce the ball off of the paddle
            if(object == paddle)
                vertical_velocity = -vertical_velocity;
            
            //if the object is the laser, the user loses a life and the game resets.
            else if(object == laser)
            {
                lives--;
                
                removeGWindow(window, laser);
                user_clicked_laser = false;
                    
                setLocation(ball, 200, 300);
                
                waitForClick();
            }  
            
            //bounce the ball off of a brick, and remove that brick from the window, user gains a point for each brick removed
            else if(strcmp(getType(object), "GRect") == 0 && object != paddle)
            {
                removeGWindow(window, object);
                
                bricks--;
                points++;
                
                updateScoreboard(window, label, points); //update the scoreboard
                
                vertical_velocity = -vertical_velocity;
            }
       }//end if/else of ball's movement
        
           
    }//end while loop
}
