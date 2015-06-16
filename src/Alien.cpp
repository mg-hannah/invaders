#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "globals.h"
#include "Missile.h"
#include "Ship.h"
#include "Alien.h"

extern Ship *ship;

Alien::Alien(int startX, int startY, int value, ALIEN_TYPE type) {

    char image_1[50];
    char image_2[50];
    char explosion[50];

    // Load the images
    if (type == ALIEN_TYPE_TOP) {
        strcpy(image_1,"images/alien_blue_1.png"); 
        strcpy(image_2,"images/alien_blue_2.png"); 
        strcpy(explosion,"images/alien_explosion.png"); 
    } else if (type == ALIEN_TYPE_MIDDLE) {
        strcpy(image_1,"images/alien_green_1.png"); 
        strcpy(image_2,"images/alien_green_2.png"); 
        strcpy(explosion,"images/alien_green_explosion.png"); 
    } else if (type == ALIEN_TYPE_BOTTOM) {
        strcpy(image_1,"images/alien_purple_1.png"); 
        strcpy(image_2,"images/alien_purple_2.png"); 
        strcpy(explosion,"images/alien_purple_explosion.png"); 
    }

    img_a = IMG_Load(image_1);
    img_a = SDL_DisplayFormatAlpha(img_a);

    img_b = IMG_Load(image_2);
    img_b = SDL_DisplayFormatAlpha(img_b);

    img_explosion = IMG_Load(explosion);
    img_explosion = SDL_DisplayFormatAlpha(img_explosion);

    // Initialise our variables
    pos.x = startX;
    pos.y = startY;
    points = value;
    visible = 1;
    hit = false;
    img_cur = img_a;
    missile = new AlienMissile();
    bomber = false;
}

int Alien::getHeight(void) {
    return img_a->h;
}

int Alien::getWidth(void) {
    return img_a->w;
}

void Alien::draw(SDL_Surface *screen) {
    if (visible) {
        SDL_BlitSurface(img_cur, NULL, screen, &pos);
    }
}

void Alien::move(int direction) {

    if (hit) return;

    if (direction == DIR_RIGHT) pos.x += 24;
    if (direction == DIR_LEFT) pos.x -= 24;

    if (img_cur == img_a) {
        img_cur = img_b;
    } else {
        img_cur = img_a;
    }
}

bool Alien::collissionDetection(void) {

    if ((ship->missile->visible) && (!hit)) {
        if (ship->missile->pos.y <= (pos.y + 32)) {
            if (((ship->missile->pos.x+MISSILE_WIDTH) >= pos.x) &&
                (ship->missile->pos.x <= (pos.x + 32))) {
                visible = 5;
                 hit = true;
                img_cur = img_explosion;
                ship->missile->visible = 0;
                return true;
            }
        }
    }

    return false;
}

int Alien::getPoints(void) {
    return points;
}

bool Alien::isHit(void) {
    return hit;
}

int Alien::isVisible(void) {
    return visible;
}

void Alien::update(void) {

    if (hit && (visible > 0)) {
        visible -= 1;
    }
}

bool Alien::atScreenEdge(int screenWidth, int direction) {

    if (direction == DIR_LEFT) {
        if (pos.x < this->getWidth()) return true;

    }

    if (direction == DIR_RIGHT) {
        if (pos.x > ((screenWidth-1) - this->getWidth())) return true;
    }

    return false;
}

void Alien::moveDown(int gap) {
    //pos.y += this->getHeight() + gap;
    pos.y += 52;
}

bool Alien::isBomber() {
    return bomber;
}

void Alien::setBomber() {
    bomber = true;
    bombTimer = rand() % 40;
}
