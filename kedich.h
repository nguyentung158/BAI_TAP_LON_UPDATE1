using namespace std;
class enemy{
private:
int demnl;
   int demdan;

    SDL_Rect eBrect;
    SDL_Rect eRect;
public:
     int eBPosX;
    int eBPosY;
     int ePosX;
      int ePosY;
    int eVel = 4;
    double eBVel =8;
    int eBVelY =3;
    enemy();
    void emove();
    void render();
    SDL_Rect getEnemyRect();
    SDL_Rect getBulletRect();
};
