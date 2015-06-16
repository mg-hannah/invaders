#include "AlienMissile.h"

class Alien {

private:
    bool hit;
    SDL_Surface *img_a;
    SDL_Surface *img_b;
    SDL_Surface *img_cur;
    SDL_Surface *img_explosion;
    int points;

public:
    SDL_Rect pos;
    int visible;
    AlienMissile *missile;
    Alien(int,int,int,char*,char*,char*);
    int getWidth(void);
    int getHeight(void);
    void draw(SDL_Surface *);
    void move(int);
    bool collissionDetection(void);
    int getPoints(void);
    bool isHit(void);
    int isVisible(void);
    void update(void);
    bool atScreenEdge(int,int);
    void moveDown(int);
};
