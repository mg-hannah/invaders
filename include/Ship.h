class Ship {
private:
    SDL_Surface *img;
    SDL_Surface *explosion;
    SDL_Surface *cur_img;
    bool movLeft;
    bool movRight;
    bool hit;
    int lives;

public:
    int visible;
    SDL_Rect pos;
    Missile *missile;
    Ship(void);
    void draw(void);
    void moveLeft(bool);
    void moveRight(bool);
    void fire(void);
    int getHeight();
    int getWidth();
    void move(void);
    bool isHit(void);
    void beenHit(void);
    void update(void);
    void reset(void);
    int getLives(void);
    SDL_Surface *getImage(void);
    void initialise(void);
};
