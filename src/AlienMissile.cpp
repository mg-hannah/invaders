#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "globals.h"
#include "AlienMissile.h"

AlienMissile::AlienMissile() {
}

void AlienMissile::move(void) {
    if (visible) {
        if (pos.y >= SCREEN_HEIGHT) {
            visible = 0;
        } else {
            pos.y += 16;
        }

   }
}
