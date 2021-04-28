#include<vector>

using namespace std;
class bullet{
private:
    int bPosX;
    int bPosY;
    int demdan;

public:
vector<int> bulletX;
    vector<int> bulletY;
       vector<SDL_Rect> bRect;
     int B_VEL = 16;
    bullet();

    void handleBullet(SDL_Event &e, int x, int y,int mw, int mh);
    void movebullet(int &i);
    void renderBullet(int &i);


};
