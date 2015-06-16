#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "Missile.h"
#include "Ship.h"
#include "globals.h"

extern Missile *missile;
extern SDL_Surface *screen;

Ship::Ship(void) {

    // Initialise position
    pos.x = SCREEN_WIDTH/2 - 16;
    pos.y = SCREEN_HEIGHT - 32 - 10;

    movLeft = false;
    movRight = false;

    // Load the image
    img = IMG_Load("images/ship.png");
    img = SDL_DisplayFormatAlpha(img);

    explosion = IMG_Load("images/alien_explosion.png");
    explosion = SDL_DisplayFormatAlpha(explosion);

    cur_img = img;

    missile = new Missile();
    visible = 1;
    hit = false;

    lives = 3;
}

void Ship::fire(void) {
    if (!hit && !missile->visible) {
        missile->visible = 1;
        missile->pos.x = pos.x + 12;
        missile->pos.y = pos.y - 5;
    }

}

int Ship::getWidth() {
    return img->w;
}

int Ship::getHeight() {
    return img->h;
}

void Ship::draw(void) {
     if (visible) {
         SDL_BlitSurface(cur_img, NULL, screen, &pos);
     }
}

void Ship::moveLeft(bool move) {
    movLeft = move;
}

void Ship::moveRight(bool move) {
    movRight = move;
}

void Ship::move(void) {
    if (movLeft) {
        pos.x -= 8;
    }

    if (movRight) {
        pos.x += 8;
    }
}

void Ship::beenHit(void) {
    cur_img = explosion;
    hit = true;
    visible = 5;
    lives--;
}

bool Ship::isHit(void) {
    return hit;
}

void Ship::update(void) {
    if (hit && (visible > 0)) {
        visible -= 1;
    }
}

void Ship::reset(void) {
    hit = false;
    visible = 1;
    pos.x = SCREEN_WIDTH/2 - 16;
    cur_img = img;
}

int Ship::getLives(void) {
    return lives;
}

SDL_Surface *Ship::getImage(void) {
    return img;
}
