#ifndef MISSILE
#define MISSILE

class Missile {
public:
    SDL_Rect pos;
    SDL_Surface *img;
    int visible;
    int colour;
    Missile(void);
    void move(void);
};
#endif /* MISSILE */
