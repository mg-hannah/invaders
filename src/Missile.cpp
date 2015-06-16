#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "globals.h"
#include "Missile.h"

Missile::Missile(void) {
    img = SDL_CreateRGBSurface(0,MISSILE_WIDTH,MISSILE_HEIGHT,32,0,0,0,0);
    colour = SDL_MapRGBA(img->format, 255, 255, 255, 0);
    SDL_FillRect(img, NULL, colour);
    img = SDL_DisplayFormatAlpha(img);
    visible = 0;
}

void Missile::move(void) {
    if (visible) {
        if (pos.y == 0) {
            visible = 0;
        } else {
            pos.y -= 16;
        }

   }
}
