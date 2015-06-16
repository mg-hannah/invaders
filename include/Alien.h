#include "AlienMissile.h"

class Alien {

private:
    bool hit;
    bool bomber;
    SDL_Surface *img_a;
    SDL_Surface *img_b;
    SDL_Surface *img_cur;
    SDL_Surface *img_explosion;
    int points;

public:
    SDL_Rect pos;
    int visible;
    AlienMissile *missile;
    Alien(int,int,int,int);
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
    bool isBomber(void);
};

#define ALIEN_TYPE_TOP    0
#define ALIEN_TYPE_MIDDLE 1
#define ALIEN_TYPE_BOTTOM 2
