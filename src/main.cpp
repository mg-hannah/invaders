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
SDL_Surface *text;
bool paused;
bool gameOver;


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

    if (aliens[0][4]->missile->visible) {
        SDL_BlitSurface(aliens[0][4]->missile->img, NULL, screen, &aliens[0][4]->missile->pos);
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
}

void collisionDetection() {

    int x,y;

    // Aliens and missile
    for (y = 0; y < 5; y++) {
        for (x = 0; x < 5; x++) {
            if (aliens[x][y]->collissionDetection()) {
		score += aliens[x][y]->getPoints();
            }
        }
    }

    // Ship and alien missiles
    if (aliens[0][4]->missile->visible && !(ship->isHit())) {
        if (aliens[0][4]->missile->pos.y >= ship->pos.y) {
            if ((aliens[0][4]->missile->pos.x >= ship->pos.x - MISSILE_WIDTH) &&
                (aliens[0][4]->missile->pos.x <= ship->pos.x + ship->getWidth())) {
                // Ship hit!
                ship->beenHit();
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
                if (!aliens[0][4]->isHit()) {
                    aliens[0][4]->missile->visible = 1;
                    aliens[0][4]->missile->pos.x = aliens[0][4]->pos.x+16;
                    aliens[0][4]->missile->pos.y = aliens[0][4]->pos.y+40;
                }
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
}

int main(int argc, char* args[])
{
    bool quit;
    SDL_Event event;
    int start;
    int x, y;
    int row, col;
    char image1[50];
    char image2[50];
    char explosion[50];
    bool dirChanged;
    bool newRow;
    int alienSpeed;

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
            strcpy(image1, "images/alien_blue_1.png");
            strcpy(image2, "images/alien_blue_2.png");
            strcpy(explosion, "images/alien_explosion.png");
        }

        if ((row == 1) || (row == 2)) {
            strcpy(image1, "images/alien_green_1.png");
            strcpy(image2, "images/alien_green_2.png");
            strcpy(explosion, "images/alien_green_explosion.png");
        }
        if ((row == 3) || (row == 4)) {
            strcpy(image1, "images/alien_purple_1.png");
            strcpy(image2, "images/alien_purple_2.png");
            strcpy(explosion, "images/alien_purple_explosion.png");
        }


        for (col = 0; col < 5; col++) {
            aliens[col][row] = new Alien((SCREEN_WIDTH/2-16)+((32+ALIEN_GAP_X)*col), 10+20+((20+ALIEN_GAP_Y)*row), 50, image1, image2, explosion);
        }
    }

    // Initialise our game variables
    alienDirection = DIR_RIGHT;
    alienSpeed = 10;
    newRow = false;
    quit = false;
    score = 0; 
    paused = false;
    gameOver = false;

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

        // Update our ship and missile
        ship->missile->move();
        ship->move();

         

	if ((!paused) && (speed++ == alienSpeed)) {
            updateAliens(&alienSpeed, &alienDirection);
	    speed = 0;
        }

        aliens[0][4]->missile->move();

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

        // Clear the screen
        SDL_FillRect(screen, NULL,0);

        // Render the screen
        drawScreen();

        // Update the screen
        SDL_Flip(screen);

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
