#include "AlienMissile.h"

typedef enum ALIEN_TYPE {
    ALIEN_TYPE_TOP,
    ALIEN_TYPE_MIDDLE,
    ALIEN_TYPE_BOTTOM
} ALIEN_TYPE;

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
    Alien(int,int,int,ALIEN_TYPE);
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
