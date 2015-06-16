#include <stdlib.h>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include "Alien.h"
#include "Missile.h"
#include "Ship.h"
#include "Shield.h"
#include "globals.h"

#define WINDOW_TITLE "Alien Invaders"

Ship *ship;
Alien *aliens[5][5];
Shield *shield[4];
SDL_Surface* screen = NULL;
int speed;
int alienDirection;
int score;
TTF_Font *font;
SDL_Surface *text, *gameOverMessage;
bool paused;
bool gameOver;
int hits;
int alienSpeed;
bool quit;

void drawGameOverMessage(void) {
    SDL_Rect msgPos;
    char message[] = "Game Over";

    msgPos.x = 300;
    msgPos.y = 300;

    SDL_Color text_color = {255, 255, 255};
    gameOverMessage = TTF_RenderText_Solid(font, message, text_color);

    SDL_BlitSurface(gameOverMessage, NULL, screen, &msgPos);
}

void drawScore(void) {
   SDL_Rect scorePos;
   char scoreString[50];

   sprintf(scoreString, "Score: %05d", score);

   SDL_Color text_color = {255, 255, 255};
   text = TTF_RenderText_Solid(font, scoreString, text_color);
   scorePos.x = SCREEN_WIDTH - text->w - 10; 
   scorePos.y = 5; 
   SDL_BlitSurface(text, NULL, screen, &scorePos);
}

void drawScreen(void) {

    int x,y;
    SDL_Rect pos;

    // Draw the ship
    ship->draw();

    // Draw the aliens  
    for (y = 0; y < 5; y++) { 
        for (x = 0; x < 5; x++) { 
            aliens[x][y]->draw(screen);
        }
    }
    
    // Draw the missiles
    if (ship->missile->visible) {
        SDL_BlitSurface(ship->missile->img, NULL, screen, &ship->missile->pos);
    }

   
    for (y = 0; y < 5; y++) { 
        for (x = 0; x < 5; x++) { 
            if (aliens[x][y]->missile->visible) {
                SDL_BlitSurface(aliens[x][y]->missile->img, NULL, screen, &aliens[x][y]->missile->pos);
            }
        }
    }

    // Draw the shields
    for (x = 0; x < 4; x++) {
        shield[x]->draw();
    }

    // Draw the score-board
    drawScore();

    // Draw the player 'lives'
    pos.y = SCREEN_HEIGHT-ship->getHeight()-2;
    for (x=1; x < ship->getLives()+1; x++) {
        pos.x = SCREEN_WIDTH-(x*((ship->getWidth()+2)));
        SDL_BlitSurface(ship->getImage(), NULL, screen, &pos);
    }

    // Game over message
    if (gameOver) {
        drawGameOverMessage();
    }
}

void collisionDetection() {

    int x,y;

    // Aliens and missile
    for (y = 0; y < 5; y++) {
        for (x = 0; x < 5; x++) {
            if (aliens[x][y]->collissionDetection()) {
		score += aliens[x][y]->getPoints();

                // If this alien is hit, need to promote the one above
                // to be the bomber.
                if (x > 0) {
                    aliens[x-1][y]->setBomber();
                }

                hits++;
            }
        }
    }

    // Ship and alien missiles
    for (y = 0; y < 5; y++) {
        for (x = 0; x < 5; x++) {
            if (aliens[x][y]->missile->visible && !(ship->isHit())) {
                if (aliens[x][y]->missile->pos.y >= ship->pos.y) {
                    if ((aliens[x][y]->missile->pos.x >= ship->pos.x - MISSILE_WIDTH) &&
                (aliens[x][y]->missile->pos.x <= ship->pos.x + ship->getWidth())) {
                // Ship hit!
                ship->beenHit();
                    }
                }
            }
        }
    }
}

void increaseAlienSpeed(int *speed) {
    if (*speed) (*speed)--;
}

void updateAliens(int *speed, int *direction) {

    int x, y;

    static bool moveDown = false;

    if (!moveDown) {
        for (y = 0; y < 5; y++) {
            for (x = 0; x < 5; x++) {
                if (aliens[x][y]->isVisible() && 
                    aliens[x][y]->atScreenEdge(SCREEN_WIDTH, *direction)) {
                    moveDown = true;
                }
            }
        }
    } else {
        moveDown = false;
    }

    if (moveDown) {
        for (y = 0; y < 5; y++) {
            for (x = 0; x < 5; x++) {
                aliens[x][y]->moveDown(ALIEN_GAP_Y);
            }
        }

        // Increase speed
        increaseAlienSpeed(speed);

        // Change direction
        if (*direction == DIR_LEFT) {
            *direction = DIR_RIGHT;
        } else {
            *direction = DIR_LEFT;
        }
    } else {
        for (y = 0; y < 5; y++) {
            for (x = 0; x < 5; x++) {
                aliens[x][y]->move(*direction);
            }
        }
    }

    // Check if due to drop bomb...
    for (y = 0; y < 5; y++) {
        for (x = 0; x < 5; x++) {
            if (aliens[x][y]->isBomber()) {
                aliens[x][y]->bombTimer--;
                if (aliens[x][y]->bombTimer == 0) {
                    // Drop bomb
                    if (!aliens[x][y]->isHit()) {
                        aliens[x][y]->missile->visible = 1;
                        aliens[x][y]->missile->pos.x = aliens[x][y]->pos.x+16;
                        aliens[x][y]->missile->pos.y = aliens[x][y]->pos.y+40;
                    }

                    // Re-set timer
                    aliens[y][x]->setBomber();
                }
            } 
        }
    }
}

void initialiseGame() {

    int x,y;

    // Initialise our game variables
    alienDirection = DIR_RIGHT;
    alienSpeed = 10;
    quit = false;
    score = 0; 
    paused = false;
    gameOver = false;
    hits = 0;

    for (y = 0; y < 5; y++) {
        for (x = 0; x < 5; x++) {
            aliens[x][y]->initialise((SCREEN_WIDTH/2-16)+((32+ALIEN_GAP_X)*y), 10+20+((20+ALIEN_GAP_Y)*x));
        }
    }

    // The bottom row are the bombers
    for (x = 0; x < 5; x++) {
        aliens[x][4]->setBomber();
    }

    ship->initialise();
}

int main(int argc, char* args[])
{
    SDL_Event event;
    int start;
    int x, y;
    int row, col;
    ALIEN_TYPE type;
    bool dirChanged;

    // Start SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    // Set up screen
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);

    // Title-bar caption
    SDL_WM_SetCaption(WINDOW_TITLE, NULL);

    // Set the keyboard repeat rate
    SDL_EnableKeyRepeat(KEY_REPEAT, KEY_REPEAT);

    // Initialise the fonts
    TTF_Init();

    // Load the freeSans font
    font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 18);
    if (font == NULL)
    {
      printf("TTF_OpenFont() Failed: %s\n", TTF_GetError());
      TTF_Quit();
      SDL_Quit();
      exit(1);
    }

    // Create our ship
    ship = new Ship();

    // Create the shields
    shield[0] = new Shield(102, 380);
    shield[1] = new Shield(240, 380);
    shield[2] = new Shield(378, 380);
    shield[3] = new Shield(516, 380);

    // Create the aliens
    for (row = 0; row < 5; row++) {

        if (row == 0) {
            type = ALIEN_TYPE_TOP;
        }
        if ((row == 1) || (row == 2)) {
            type = ALIEN_TYPE_MIDDLE;
        }
        if ((row == 3) || (row == 4)) {
            type = ALIEN_TYPE_BOTTOM;
        }


        for (col = 0; col < 5; col++) {
            aliens[row][col] = new Alien((SCREEN_WIDTH/2-16)+((32+ALIEN_GAP_X)*col), 10+20+((20+ALIEN_GAP_Y)*row), 50, type);
        }
    }

    initialiseGame();

    while (!quit) {

        start = SDL_GetTicks();

        // Poll the keyboard
        while (SDL_PollEvent(&event)) {

            switch(event.type) {
                case SDL_QUIT: 	quit = true;
				break;
                case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_LEFT) {
					ship->moveLeft(true);
				}
				if (event.key.keysym.sym == SDLK_RIGHT) {
					ship->moveRight(true);
				}
				if (event.key.keysym.sym == SDLK_SPACE) {
                                    ship->fire();
				}
				if (event.key.keysym.sym == SDLK_ESCAPE) {
				    quit = true;
				}
				if (event.key.keysym.sym == SDLK_n) {
                                    if (gameOver) initialiseGame();
				}
                                paused = false;
				break;
		case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_LEFT) {
					ship->moveLeft(false);
				}
				if (event.key.keysym.sym == SDLK_RIGHT) {
					ship->moveRight(false);
				}
            }

        }  

        if (!gameOver) {

            // Update our ship and missile
            ship->missile->move();
            ship->move();

	    if ((!paused) && (speed++ == alienSpeed)) {
                updateAliens(&alienSpeed, &alienDirection);
	        speed = 0;
            }

            for (col=0; col < 5; col++) {
                for (row=0; row < 5; row++) {
                    aliens[row][col]->missile->move();
                }
            }

            collisionDetection();

            for (y = 0; y < 5; y++) {
                for (x = 0; x < 5; x++) {
		    aliens[x][y]->update();
                }
            }
            ship->update();

            if ((ship->isHit()) && (ship->visible == 0)) {
                ship->reset();
                paused = true;
            }

            // Check if game over
            if ((hits == 25) || (ship->getLives() == 0)) {
                gameOver = true;
            }

            // Clear the screen
            SDL_FillRect(screen, NULL,0);

            // Render the screen
            drawScreen();

            // Update the screen
            SDL_Flip(screen);
        }

        // Delay for the appropriate amount of time
        if (1000/FPS > (SDL_GetTicks() - start)) {
	    SDL_Delay(1000/FPS - (SDL_GetTicks() - start));
        }

    }

    printf("Exiting...\n");

    // Free the loaded image
    SDL_FreeSurface(screen);

    // Quit SDL
    SDL_Quit();

    return 0;
}
