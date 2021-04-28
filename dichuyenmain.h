using namespace std;
class Dot{
private:
    int mPosX;
    int mPosY;
    //van toc dau cham
    int demnl;
    SDL_Rect mainCollider;
public:
     int mVelX;
    int mVelY;
    static const int DOT_VEL = 10;

    Dot();
    void handleEvent(SDL_Event &e,bool &checkPause);
    void move();
    void render();
    int getPosX();
    int getPosY();
    SDL_Rect getMainRect();
    void freeMainShip();
};
