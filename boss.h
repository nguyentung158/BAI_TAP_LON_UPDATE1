using namespace std;
class boss{
private:
    int demdan1;
    int demnl;
    int bossPosX;
    int bossPosY;
    int BBulletX ;
    int BBulletY ;
    SDL_Rect bossRect;
    SDL_Rect BossBulletRect;
public:
     int bossVel =1 ;
    int velY =5;
    boss();
    void moveBossBullet();
    void bossmove();
    void render();
    SDL_Rect getbossRect();
    SDL_Rect getBossBulletRect();
    int getBossVel();
};
