class Shield {
public:
    SDL_Rect pos;
    SDL_Surface *img;
    Shield(int, int);
    void draw(void);
    int getWidth(void);
};
