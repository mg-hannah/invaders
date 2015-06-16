#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "Shield.h"

extern SDL_Surface *screen;

Shield::Shield(int x, int y) {
    img = IMG_Load("images/shield.png");
    img = SDL_DisplayFormatAlpha(img);
    printf("Shield width: %d", img->w);

    pos.x = x;
    pos.y = y;
}

void Shield::draw(void) {
    SDL_BlitSurface(img, NULL, screen, &pos);
}
