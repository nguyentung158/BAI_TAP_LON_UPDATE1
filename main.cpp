#include<iostream>
#include<fstream>
#include<sstream>
#include<string.h>
#include<SDL.h>
#include<time.h>
#include<vector>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include"Texture.h"
#include"dichuyenmain.h"
#include"kedich.h"
#include"mainbullet.h"
#include<SDL_mixer.h>
#include"boss.h"
#include"MouseMenu.h"

const int SCREEN_WIDTH = 1240;
const int SCREEN_HEIGHT =680;

Mix_Music* bossTheme =NULL;
Mix_Music* gmusic =NULL;
Mix_Music* menuMusic = NULL;
Mix_Chunk* button = NULL;
Mix_Chunk* danLaser = NULL;
Mix_Chunk* VuNoVaCham = NULL;
Mix_Chunk* bossLaser =NULL;
Mix_Chunk* gameWin = NULL;
Mix_Chunk* gameLose =NULL;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

Texture dandich[3];
Texture nhienlieu1[5];
Texture nhienlieumain[5];
Texture comingBossBg;
Texture background;
Texture mainShip;
Texture enemyShip;
Texture laser[3];
Texture gameOverBackGround;
Texture menuBg;
Texture explosion[6];
Texture bossExplosion[6];
Texture Boss;
Texture bomboss[3];
Texture HPBoss[6];
Texture wingamebg;
Texture bgLayer;
Texture BGLAYERR[8];
Texture Guide;
Texture guimenu;
Texture text1;
Texture PressSpace;
Texture PressEsc;
Texture startMenu[2];
Texture quitMenu[2];
Texture SpaceShip[2];
Texture HiScore;
Texture pause[2];
mouse MenuMouse;
TTF_Font* font1;
TTF_Font* font2;
TTF_Font* font3;
SDL_Color textColor1 ={255,0,0};
SDL_Color textColor2 ={0xff,0xff,0xff};

bool init();
bool loadMedia();
void close();
bool checkCollision();
//bool kiemtravacham();
Texture::Texture(){
    mHeight =0; mWidth = 0;
    mTexture = NULL;
}
Texture::~Texture(){
 free();
}
bool Texture::loadFromFile(string path){
    free();
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    SDL_Texture* newtexture = NULL;
    if(loadedSurface == NULL){
        IMG_GetError();
    }
     else{
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format,0xff,0,0));

        newtexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newtexture == NULL){
            SDL_GetError();
        }else{
            mHeight = loadedSurface->h;
            mWidth = loadedSurface->w;
        }
        SDL_FreeSurface(loadedSurface);
     }
     mTexture = newtexture;
    return mTexture!= NULL;
}
bool Texture::loadFromRenderText(string textureText, SDL_Color textColor, TTF_Font* font){
    free();
    SDL_Surface* TextSurface = TTF_RenderText_Solid(font,textureText.c_str(), textColor);
    if(TextSurface ==NULL){
        TTF_GetError();
    } else {

        mTexture =SDL_CreateTextureFromSurface(gRenderer, TextSurface);
        if(mTexture == NULL){
            SDL_GetError();
        }
         mHeight = TextSurface ->h;
            mWidth = TextSurface ->w;
    }
    SDL_FreeSurface(TextSurface);
    return mTexture != NULL;
}
void Texture::free(){
    SDL_DestroyTexture(mTexture);
    mHeight =0;
    mWidth =0;
}
void Texture::render(int x, int y, SDL_Rect* clips, double angle, SDL_Point* center, SDL_RendererFlip flip){
    SDL_Rect renderQuad ={x,y,mWidth, mHeight};
    if(clips != NULL){
        renderQuad.w = clips->w;
        renderQuad.h = clips->h;
    }
    SDL_RenderCopyEx(gRenderer,mTexture,clips, &renderQuad, angle, center, flip);

}
int Texture::getHeight(){
return mHeight;
}
int Texture::getWidth(){
    return mWidth;
}

Dot::Dot(){
     mPosX = 30;
    mPosY =SCREEN_HEIGHT/2-10;
    //khoi tao van toc
    mVelX = 0;
    mVelY =0;
    mainCollider.h =58;
    mainCollider.w =131;
    demnl =0;
}
void Dot::handleEvent(SDL_Event& e,bool &checkPause){
     //neu ta an 1 phim

    if(e.type == SDL_KEYDOWN && e.key.repeat == 0){


        switch(e.key.keysym.sym){

            case SDLK_w:mVelY -= DOT_VEL; break;
            case SDLK_s: mVelY += DOT_VEL; break;
            case SDLK_d: mVelX += DOT_VEL; break;
            case SDLK_a: mVelX -= DOT_VEL;break;
            case SDLK_p:
                Mix_PauseMusic();
                checkPause =true;
                while(checkPause){


                        menuBg.render(0,0,NULL);
                        for(int i=0;i<=1;i++){
                        SpaceShip[i].render(SCREEN_WIDTH/2-350,50,NULL);
                        pause[1].render((SCREEN_WIDTH- pause[1].getWidth())/2, 0,NULL);
                        SDL_RenderPresent(gRenderer);}
                    while(SDL_PollEvent(&e) != 0){
                        if(e.type ==SDL_KEYDOWN){
                            if(e.key.keysym.sym == SDLK_p){
                                    Mix_ResumeMusic();
                                checkPause=false;
                                break;
                            }
                        }
                    }
                }
                break;
        }

    } else if(e.type == SDL_KEYUP&& e.key.repeat ==0 ){

        switch(e.key.keysym.sym){
            case SDLK_w:mVelY += DOT_VEL; break;
            case SDLK_s: mVelY -= DOT_VEL; break;
            case SDLK_d: mVelX -= DOT_VEL; break;
            case SDLK_a: mVelX += DOT_VEL;break;
    }

     }

}
void Dot::move(){
    mPosX += mVelX;
    	mainCollider.x = mPosX+22;
    mainCollider.w =131;
    mainCollider.h =58;
    //If the dot collided or went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + mainShip.getWidth() > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
		mainCollider.x = mPosX+22;
    }

    //Move the dot up or down
    mPosY += mVelY;
	mainCollider.y = mPosY + 14;

    //If the dot collided or went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + mainShip.getHeight() > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
		mainCollider.y = mPosY+14;
    }

}
void Dot::render(){
    mainShip.render(mPosX,mPosY,NULL);
    demnl++;
    nhienlieumain[demnl/4].render(mPosX-55,mPosY,NULL);

    if(demnl == 19){
        demnl=0;
    }
}
int Dot::getPosX(){
    return mPosX;
}
int Dot::getPosY(){
    return mPosY;
}
SDL_Rect Dot::getMainRect(){
    return mainCollider;
}
void Dot::freeMainShip(){
    mPosX =30;
    mPosY =SCREEN_HEIGHT/2-10;
    mainCollider.x =0;
    mainCollider.y =0;
    mVelX = 0;
    mVelY =0;
}
enemy::enemy(){

    ePosX =SCREEN_WIDTH;
    eBPosX =ePosX +5;

    ePosY  = rand() % (SCREEN_HEIGHT  - enemyShip.getHeight());

    eRect.h =enemyShip.getHeight();
    eRect.w = enemyShip.getWidth();
    demnl=0;
    demdan =0;
eBrect.h =17;
eBrect.w =31;

}

void enemy::emove(){

if(ePosX >= SCREEN_WIDTH-10){
    eBPosY = ePosY + 10;

}
    eRect.x =ePosX;
    eRect.y =ePosY;
   eBrect.x =eBPosX+47;
    eBrect.y =eBPosY+52;
}
void enemy::render() {

        ePosX -= eVel;
        eRect.x = ePosX;

    if(ePosX<= 0){
        ePosX  =SCREEN_WIDTH;

        eRect.x =ePosX;
        ePosY = rand() % (SCREEN_HEIGHT  - enemyShip.getHeight());
    }

    enemyShip.render(ePosX,ePosY,NULL);
    nhienlieu1[demnl/4].render(ePosX + enemyShip.getWidth()-25,ePosY -20,NULL);
    demnl++;
    if(demnl==19){
        demnl=0;
    }


        eBPosX -= eBVel;

        if(eBVel >= 9){
                eBPosY += eBVelY;
            if(eBPosY <= 0){
                 eBVelY = -eBVelY;
            } else if(eBPosY >= SCREEN_HEIGHT){
                 eBVelY = -eBVelY;
            }
        }

        eBrect.x =eBPosX +47;
        eBrect.y =eBPosY +52;
        if(eBPosX <=0 && ePosX >=SCREEN_WIDTH-50){

            eBPosX =ePosX +5;
            if(  eBPosX ==ePosX +5){
                Mix_PlayChannel(9,danLaser,0);
            }
        eBPosY = ePosY + 10;
        eBrect.y =eBPosY +52;
    }

        dandich[demdan/4].render(eBPosX,eBPosY,NULL);
        demdan++;
        if(demdan ==11){
            demdan =0;
        }



}

SDL_Rect enemy::getEnemyRect(){
    return eRect;
}
SDL_Rect enemy::getBulletRect(){
    return eBrect;
}
boss::boss(){
    demnl =0;
    demdan1 =0;
   bossPosX =0;
    bossPosY  = 0;
    BBulletX = 0;
    BBulletY = 0;
    bossRect.w = Boss.getWidth();
    bossRect.h = Boss.getHeight();
    BossBulletRect.h =bomboss[0].getHeight();
    BossBulletRect.w = bomboss[0].getWidth();
}

void boss::bossmove(){
    bossPosX =SCREEN_WIDTH ;
    bossPosY  = SCREEN_HEIGHT/2 - bossRect.h/2;

    bossRect.w = Boss.getWidth();
    bossRect.h = Boss.getHeight();
    BBulletX =SCREEN_WIDTH -Boss.getWidth() - bomboss[0].getWidth() +50;
    BossBulletRect.x =BBulletX+50;
    BBulletY = bossPosY + 14;
    BossBulletRect.y =BBulletY+53;
    BossBulletRect.h = 31;
    BossBulletRect.w = 168;

}
void boss::render(){


    bossPosX-= bossVel;
    bossRect.x = bossPosX;

    BossBulletRect.x =BBulletX+50 ;
    BossBulletRect.y =BBulletY+53;
    if(bossPosX <=SCREEN_WIDTH  - Boss.getWidth() -50){
 if(BBulletX >= bossRect.x -bomboss[0].getWidth() -51  ) Mix_PlayChannel(6,bossLaser,0);

        bossVel =5;
        BBulletX -= 12;

        bossPosX+= bossVel;
        bossRect.x =bossPosX;
        bossPosY -= velY;
        bossRect.y = bossPosY;

        if(bossPosY <= 0){
            velY =- velY;

        } else if(bossPosY + Boss.getHeight() >= SCREEN_HEIGHT){
            velY=-velY;
        }
         bossRect.y = bossPosY;
   bomboss[demdan1/4].render(BBulletX ,BBulletY  ,NULL);
   demdan1++;
   if(demdan1 ==11){
    demdan1=0;
   }
    }
    if(BBulletX<= 0 ){

        BBulletX =bossRect.x -bomboss[0].getWidth() +50;
        BossBulletRect.x = BBulletX+50;

        BBulletY = bossRect.y+ 14;
        BossBulletRect.y =BBulletY+53;

    }

    Boss.render(bossPosX,bossPosY,NULL);
    nhienlieu1[demnl/4].render(bossPosX +Boss.getWidth()-20, bossPosY +38,NULL);
    nhienlieu1[demnl/4].render(bossPosX +Boss.getWidth()-20, bossPosY +72,NULL);
    demnl++;
if(demnl ==19){
    demnl =0;
}

}
SDL_Rect boss::getbossRect()  {
    return bossRect;
}
SDL_Rect boss::getBossBulletRect(){
    return BossBulletRect;
}
int boss::getBossVel(){
    return bossVel;
}
bullet::bullet(){
    bPosX = 0;
    bPosY =0;
    demdan =0;
}

void bullet::handleBullet(SDL_Event& e,int  mx,int my, int mw, int mh ){

    if(e.type == SDL_MOUSEBUTTONDOWN ){

        Mix_PlayChannel(0,danLaser,0);

        bulletX.push_back( mx +80);
        bulletY.push_back((my-10 ));
        bRect.push_back({0,0,0,0});

        }

}
void bullet::movebullet(int &i){
    bulletX.at(i) += B_VEL;
    bRect.at(i).x = bulletX.at(i) +42;
    bRect.at(i).y = bulletY.at(i) +52;
     bRect.at(i).w = 32;
     bRect.at(i).h = 16;

    if(bulletX.at(i) + laser[0].getWidth()>=  SCREEN_WIDTH -10){
if(bRect.size()>1){
        bulletX.erase(bulletX.begin() );
        bulletY.erase(bulletY.begin());
        bRect.erase(bRect.begin());
}
    }
}
void bullet::renderBullet(int &i) {

    laser[demdan/4].render(bulletX.at(i) ,bulletY.at(i),NULL);
    demdan++;
    if(demdan ==11){
        demdan=0;
    }
}
mouse::mouse(){
    Position.x =0;
    Position.y =0;
    CurrentQuit =0;
    CurrentStart =0;
}
mouse::~mouse(){

}
void mouse::setPoisition(int x, int y){
    Position.x =x;
    Position.y =y;
}
void mouse::handleEvent(SDL_Event& e, bool& again, bool & checkMenu){
    if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP){
        //Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );
        bool insideStart =true;
        bool insideQuit =true;
        bool insideQues =true;
        if(x < 0){
            insideQues =false;
        }
        if(x >guimenu.getWidth()){
            insideQues =false;
        }
        if(y < 0){
            insideQues =false;
        }
        if(y > guimenu.getHeight()){
            insideQues =false;
        }
        if(x < Position.x){
            insideQuit =false;
            insideStart =false;
        }  if(x > Position.x + startMenu[0].getWidth()){
            insideQuit =false;
            insideStart =false;
        } if(y < Position.y  ){
            insideQuit =false;
            insideStart =false;

        }  if(y > Position.y + startMenu[0].getHeight() ){
            insideStart =false;
        }  if(y < Position.y + startMenu[0].getHeight()){
            insideQuit =false;
        }  if(y >Position.y + startMenu[0].getHeight() + quitMenu[0].getHeight()){
            insideQuit =false;
        }
        if(!insideQuit){
            CurrentQuit =0;
        } else if(insideQuit){
            CurrentQuit =1;
        }
        if(!insideStart){
            CurrentStart =0;
        } else if(insideStart){
            CurrentStart =1;
        }

       if(e.type == SDL_MOUSEBUTTONDOWN && insideQues ==true){
            guimenu.loadFromRenderText("Back",textColor2,font1);
                Guide.render(0,0,NULL);
       guimenu.render(0,0,NULL);
        SDL_RenderPresent(gRenderer);
        while(insideQues ==true){

            while(SDL_PollEvent(&e ) != 0){

                if(e.type == SDL_MOUSEBUTTONDOWN && insideQues ==true){
                        guimenu.loadFromRenderText("Guide",textColor1,font1);
                        insideQues =false;
                        break;
                    }

                }
            }

        }

        if(e.type == SDL_MOUSEBUTTONDOWN && insideStart == true){
                Mix_HaltMusic();
        Mix_PlayChannel(7,button,0);
            again =true;
        }
        if(e.type ==SDL_MOUSEBUTTONDOWN && insideQuit ==true){
                      Mix_HaltMusic();
        Mix_PlayChannel(7,button,0);
            checkMenu =false;
        }

    }
}
void mouse::render(){
    startMenu[CurrentStart].render(Position.x,Position.y,NULL);
    quitMenu[CurrentQuit].render(Position.x,Position.y + startMenu[CurrentStart].getHeight(),NULL);
}
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Space Ship", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
				if(TTF_Init() == -1){
                    success =false;
				}
			}
		}
	}

	return success;
}

bool loadMedia(){
     font1 =TTF_OpenFont("prstartk.ttf", 16);
    font2 =TTF_OpenFont("prstartk.ttf", 40);
    font3 =TTF_OpenFont("prstartk.ttf", 70);
    bool success =true;
    if(!mainShip.loadFromFile("mainship.png")){
        success =false;
    }
    if(!background.loadFromFile("spaceback1.png")){
        success =false;
    }

    if(!enemyShip.loadFromFile("enemypic.png")){
        success =false;
    }
    if(!laser[0].loadFromFile("Laser Sprites/08.png")){
        success =false;
    }
    Guide.loadFromFile("menu.png");
    BGLAYERR[0].loadFromFile("2.Space_1280x720 - Back2.png");
    BGLAYERR[1].loadFromFile("3.Space_1280x720 - Back3.png");
    BGLAYERR[2].loadFromFile("4.Space_1280x720 - Back4.png");
    BGLAYERR[3].loadFromFile("5.Space_1280x720 - Back5.png");
    BGLAYERR[4].loadFromFile("6.Space_1280x720 - Back6.png");
    BGLAYERR[5].loadFromFile("7.Space_1280x720 - Back7.png");
    BGLAYERR[6].loadFromFile("8.Space_1280x720 - Back8.png");
    BGLAYERR[7].loadFromFile("9.Space_1280x720 - Back9.png");
    laser[1].loadFromFile("Laser Sprites/09.png");
    laser[2].loadFromFile("Laser Sprites/10.png");
    dandich[0].loadFromFile("Laser Sprites/03.png");
    dandich[1].loadFromFile("Laser Sprites/04.png");
    dandich[2].loadFromFile("Laser Sprites/07.png");
    if(!gameOverBackGround.loadFromRenderText("YOU LOSE! ",textColor1, font3)){
            TTF_GetError();
        success =false;
    }
    if(!explosion[0].loadFromFile("explosion0.png")){
        success =false;
    }
    if(!Boss.loadFromFile("bosspic.png")){
        success =false;
    }
    gmusic = Mix_LoadMUS("music_pioxonaq_agressive_lightning.mp3");
    if(gmusic == NULL){
        Mix_GetError();
        success =false;
    }
    bgLayer.loadFromFile("10.Space_1280x720 - Stars.png");
    menuBg.loadFromFile("menuu.png");
    nhienlieumain[0].loadFromFile("PNG/1/1.png");
    nhienlieumain[1].loadFromFile("PNG/1/2.png");
    nhienlieumain[2].loadFromFile("PNG/1/3.png");
    nhienlieumain[3].loadFromFile("PNG/1/4.png");
    nhienlieumain[4].loadFromFile("PNG/1/5.png");
    nhienlieu1[0].loadFromFile("PNG/2/1.png");
    nhienlieu1[1].loadFromFile("PNG/2/2.png");
    nhienlieu1[2].loadFromFile("PNG/2/3.png");
    nhienlieu1[3].loadFromFile("PNG/2/4.png");
    nhienlieu1[4].loadFromFile("PNG/2/5.png");
    guimenu.loadFromRenderText("Guide", textColor1, font1);
    pause[0].loadFromRenderText("Press p to pause the game", textColor1,font1);
    pause[1].loadFromRenderText("Press p to continue the game", textColor1,font1);
    SpaceShip[0].loadFromRenderText("SPACE SHIP", textColor2,font3);
    SpaceShip[1].loadFromRenderText("SPACE SHIP", textColor1, font3);
    startMenu[0].loadFromRenderText("Start game", textColor2, font2);
    startMenu[1].loadFromRenderText("Start game", textColor1,font2);
    quitMenu[0].loadFromRenderText("Quit game", textColor2,font2);
    quitMenu[1].loadFromRenderText("Quit game", textColor1,font2);
    PressEsc.loadFromRenderText("Press Esc to return Menu", textColor1, font1);
    PressSpace.loadFromRenderText("Press Space to play again ", textColor1, font1);
    bossTheme =Mix_LoadMUS("music_zapsplat_breaking_boundaries.mp3");
    danLaser = Mix_LoadWAV("laser-gun-19sf.mp3");
    VuNoVaCham = Mix_LoadWAV("Explosion+9.mp3");
    bossLaser = Mix_LoadWAV("zapsplat_cartoon_laser_shoot_64776.mp3");
    gameWin = Mix_LoadWAV("cartoon_success_fanfair.mp3");
    gameLose = Mix_LoadWAV("zapsplat_multimedia_game_sound_fun_arcade_organ_short_negative_fail_lose_003_54276.mp3");
    button = Mix_LoadWAV("tspt_game_button_04_040.mp3");
    menuMusic = Mix_LoadMUS("music_zapsplat_game_music_action_uplifting_electro_house_anthem_retro_melody_026.mp3");
    bomboss[0].loadFromFile("Laser Sprites/11.png");
    bomboss[1].loadFromFile("Laser Sprites/12.png");
    bomboss[2].loadFromFile("Laser Sprites/13.png");
    comingBossBg.loadFromRenderText("BOSS IS COMING!!!", textColor1,font2);
    wingamebg.loadFromRenderText("YOU WIN! ",textColor1, font3);
    HPBoss[0].loadFromFile("hp0.png");
    HPBoss[1].loadFromFile("hp1.png");
    HPBoss[2].loadFromFile("hp2.png");
    HPBoss[3].loadFromFile("hp3.png");
    HPBoss[4].loadFromFile("hp4.png");
    HPBoss[5].loadFromFile("hp5.png");
    explosion[1].loadFromFile("explosion1.png");
    explosion[2].loadFromFile("explosion2.png");
    explosion[3].loadFromFile("explosion3.png");
    explosion[4].loadFromFile("explosion4.png");
    explosion[5].loadFromFile("explosion5.png");
    bossExplosion[0].loadFromFile("0.png");
    bossExplosion[1].loadFromFile("1.png");
    bossExplosion[2].loadFromFile("2.png");
    bossExplosion[3].loadFromFile("3.png");
    bossExplosion[4].loadFromFile("4.png");
    bossExplosion[5].loadFromFile("5.png");


    return success;

}

void close(){
    guimenu.free();
    Mix_FreeChunk(button);
    button =NULL;
    Mix_FreeMusic(menuMusic);
    menuMusic =NULL;
    Mix_FreeMusic(bossTheme);
    bossTheme =NULL;
    Mix_FreeMusic(gmusic);
    gmusic = NULL;
    Mix_FreeChunk(danLaser);
    danLaser = NULL;
    Mix_FreeChunk(VuNoVaCham);
    VuNoVaCham = NULL;
    Mix_FreeChunk(bossLaser);
    bossLaser =NULL;
    Mix_FreeChunk(gameLose);
    Mix_FreeChunk(gameWin);
    gameLose =NULL;
    gameWin =NULL;
    //Free loaded images
	mainShip.free();
    background.free();
    bgLayer.free();
    for(int i =0; i<= 7;i++){
        BGLAYERR[i].free();
    }
    enemyShip.free();
    laser[0].free();
    laser[1].free();
    laser[2].free();
    Guide.free();
    gameOverBackGround.free();
    Boss.free();
    bomboss[0].free();
    bomboss[1].free();
    bomboss[2].free();
    comingBossBg.free();
    text1.free();
    PressEsc.free();
    PressSpace.free();
    menuBg.free();
    HiScore.free();
    for(int i =0;i<=1;i++){
    SpaceShip[i].free();
    startMenu[i].free();
    quitMenu[i].free();
    pause[i].free();
    }
    for(int i=0;i<=5;i++){
        HPBoss[i].free();
        bossExplosion[i].free();
        explosion[i].free();
    }
    for(int i=0;i<=4;i++){
        nhienlieu1[i].free();
        nhienlieumain[i].free();
    }
    dandich[0].free();
    dandich[1].free();
    dandich[2].free();
    wingamebg.free();
	//Destroy window

	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	SDL_DestroyRenderer( gRenderer );
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}
bool checkCollision(SDL_Rect a, SDL_Rect b){
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //tinh toan cac mat rect a
    leftA =a.x;
    rightA = a.x + a.w;
    topA =a.y;
    bottomA =a.y + a.h;
    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if(leftA >= rightB){
        return false;
    } else if(rightA <= leftB){
        return false;
    } else if(topA >= bottomB){
        return false;
    } else if(bottomA <= topB){
        return false;
    }
    return true;
}
int main(int argc, char* args[]){



srand(time(0));
init();
loadMedia();
int highscore;

string Hiscore = "High Score: ";
string mark1 = "Score: ";
string mark3;
string mark2;
bool checkPause =false;
bool checkAgain =true;
bool checkMenu = true;
  Dot mainship ;
  int ev[4];
int diemso;
double velBg;
SDL_Event e;
while(checkMenu ){
        if(Mix_PlayingMusic() ==0){
            Mix_PlayMusic(menuMusic,-1);
        }
    fstream ff;
    ff.open("hiScore.txt",ios::in);
    string data;
    string line;
    while(!ff.eof()){
        getline(ff,line);
    }

    data+= line;
    ff.close();
    stringstream ss(data);

    ss>>highscore;


    HiScore.loadFromRenderText(Hiscore + data, textColor1,font1);
         SDL_SetRenderDrawColor(gRenderer,0,0xff,0xff,0xff);
        SDL_RenderClear(gRenderer);
        menuBg.render(0,0,NULL);
        bool again =false;
        bool quit =false;

        MenuMouse.setPoisition((SCREEN_WIDTH-startMenu[0].getWidth())/2, (SCREEN_HEIGHT-startMenu[0].getHeight())/2);
       while(SDL_PollEvent(&e) != 0){
            MenuMouse.handleEvent(e, again, checkMenu);
            if(e.type == SDL_QUIT){
                    checkMenu =false;
            }
       }
        MenuMouse.render();
        HiScore.render((SCREEN_WIDTH-HiScore.getWidth())/2,20,NULL);
        guimenu.render(0,0,NULL);
        for(int i=0;i<=9;i++){
         SpaceShip[i/5].render((SCREEN_WIDTH-SpaceShip[0].getWidth())/2,120,NULL);
         SDL_RenderPresent(gRenderer);
        }

while(again){
  vector <enemy> enemyship;
enemyship.resize(4);
for(int i =0; i<enemyship.size();i++){
enemyship.at(i).ePosY = rand() % (SCREEN_HEIGHT  - enemyShip.getHeight());
ev[i] =enemyship.at(i).eVel;
}
         diemso =0;
         velBg =1.16;
        int checkBoss =0;

         vector< vector <int> > coli;
         vector< vector <int> > coliX;
         vector< vector <int> > coliY;
            coli.resize(4);
            coliX.resize(4);
            coliY.resize(4);
checkAgain =true;
bool checkMainShipCollision =false;
bool checkBossTheme =true;



int demBackGround =0;
int finalBackground =0;
double scrollingOffSet =0;
double scrollingOffSet1 =0;
    bullet lazer;

boss mainBoss;


mainBoss.bossmove();
if(Mix_PlayingMusic()){
    Mix_HaltMusic();
}
int z =0;


    while(!quit){
            cout << mainship.getMainRect().y<< endl;
if(Mix_PlayingMusic() == 0 && demBackGround <3)
                   Mix_PlayMusic(gmusic,-1);

z++;


while(SDL_PollEvent(&e) != 0){


            if(e.type == SDL_QUIT){
        quit=true;

        checkAgain =false;
        again =false;
     //   checkMenu =false;
     Mix_HaltMusic();
           } else  {
           mainship.handleEvent(e, checkPause);
         lazer.handleBullet(e,mainship.getPosX(), mainship.getPosY(),mainShip.getWidth(), mainShip.getHeight());
  }
        if(e.type == 770 && z >1){
            for(int i=0;i<enemyship.size();i++){
            enemyship.at(i).eVel =0;
            }
            mainBoss.bossVel=0;
            velBg =0;
        } else if(e.type =512 && velBg == 0) {
            velBg =1.16;
            mainBoss.bossVel =6;
            for(int i=0;i<enemyship.size();i++){
            enemyship.at(i).eVel =ev[i];
            }
        }


      }

        SDL_SetRenderDrawColor(gRenderer,0,0xff,0xff,0xff);
        SDL_RenderClear(gRenderer);
        if(finalBackground <=2){
        scrollingOffSet-= velBg;
        scrollingOffSet1 -= velBg/3;
        }
        if(scrollingOffSet1 < -bgLayer.getWidth()){
             scrollingOffSet1 =0;
        }
        if(scrollingOffSet < -background.getWidth()){
        scrollingOffSet =0;
        finalBackground++;
        for(int i=0; i< enemyship.size();i++){
        if(enemyship.at(i).eVel<=8)
        enemyship.at(i).eVel+=enemyship.at(i).eVel/2;
         ev[i] =enemyship.at(i).eVel;
         enemyship.at(i).eBVel += 0.5;
        }
        demBackGround++;
    }

        background.render(scrollingOffSet,0,NULL);
        background.render(scrollingOffSet + background.getWidth(),0,NULL);
        for(int i =0; i<= 7;i++){
        BGLAYERR[i].render(0,0,NULL);
         }
        bgLayer.render(scrollingOffSet1,0,NULL);
        bgLayer.render(scrollingOffSet1 + background.getWidth(),0,NULL);
        mark2 = to_string(diemso);
        mark3 =mark1 +mark2;
        text1.loadFromRenderText(mark3, textColor1,font1);
        if(diemso > highscore)
        HiScore.loadFromRenderText(Hiscore + mark2 , textColor1,font1);
        for(int i=0;i<enemyship.size();i++){
        enemyship.at(i).emove();
        enemyship.at(i).render();
        }
        mainship.move();
        mainship.render();



        //xu ly va ve dan
        for(int j =0; j<lazer.bulletX.size();j++){

        lazer.movebullet(j);
        lazer.renderBullet(j);

        for(int i=0;i<enemyship.size();i++){
          if(demBackGround >=3){


           if(checkCollision(lazer.bRect.at(j), mainBoss.getbossRect())   ){
                if(  mainBoss.bossVel==5){
            Mix_PlayChannel(5,VuNoVaCham,0);
        int x =lazer.bulletX[j];
        int y = lazer.bulletY[j];

            coli[3].push_back(3);
            coliX[3].push_back(x+ 75);
            coliY[3].push_back(y+48);

            lazer.bulletX.erase(lazer.bulletX.begin() +j);
         lazer.bulletY.erase(lazer.bulletY.begin()+j );
        lazer.bRect.erase(lazer.bRect.begin() +j);
            checkBoss++;
            break;
                }

        }}
       if(checkCollision(lazer.bRect.at(j), enemyship.at(i).getEnemyRect())  ){

            Mix_PlayChannel(1,VuNoVaCham,0);
            enemyship.at(i).ePosX=SCREEN_WIDTH;
            enemyship.at(i).ePosY = rand() % (SCREEN_HEIGHT  - enemyShip.getHeight());
            enemyship.at(i).eBPosX = enemyship.at(i).ePosX +5;
         enemyship.at(i).eBPosY = enemyship.at(i).ePosY + 10;
          lazer.bulletX.erase(lazer.bulletX.begin() +j);
         lazer.bulletY.erase(lazer.bulletY.begin()+j );
        lazer.bRect.erase(lazer.bRect.begin() +j);
            coli[0].push_back(0);
            coliX[0].push_back(enemyship.at(i).getEnemyRect().x);
            coliY[0].push_back(enemyship.at(i).getEnemyRect().y);
        diemso+=10;
break;
        }

        }


        }
        //ve vu no

  for(int i =0;i < coli[0].size();i++){
        if(coli[0][i] <=17){
        explosion[coli[0][i]/3].render(coliX[0][i],coliY[0][i],NULL);
        coli[0][i]++;
        }
        if(coli[0][i] >17){
             coli[0].erase(coli[0].begin());
             coliX[0].erase(coliX[0].begin());
             coliY[0].erase(coliY[0].begin());
        }
        }

for(int i =0;i<enemyship.size();i++){
        if(checkCollision(mainship.getMainRect(), enemyship.at(i).getEnemyRect())){
                Mix_PlayChannel(4,VuNoVaCham,0);

       checkMainShipCollision =true;

        }
        if(checkCollision(mainship.getMainRect(), enemyship.at(i).getBulletRect())){
            Mix_PlayChannel(4,VuNoVaCham,0);
            checkMainShipCollision =true;
        }
}

         if(demBackGround >=3){
         if(mainBoss.getbossRect().x >SCREEN_WIDTH  - Boss.getWidth() +10 ){
                comingBossBg.render(SCREEN_WIDTH/2 - comingBossBg.getWidth()/2,SCREEN_HEIGHT/2 - comingBossBg.getHeight()/2,NULL);
                    for(int i =0; i<enemyship.size() ;i++){
                    enemyship.at(i).eVel=0;
                    enemyship.at(i).ePosX =SCREEN_WIDTH;
                    enemyship.at(i).eBVel =0;

                    enemyship.at(i).eBPosX  =enemyship.at(i).ePosX +5; ;
                    }

         }
          if(mainBoss.getbossRect().x <=SCREEN_WIDTH  - Boss.getWidth() +2){
                for(int a =0; a <enemyship.size();a++){
             enemyship.at(a).eVel=9;
             enemyship.at(a).eBVel =15;

             }

          }
                if(checkBossTheme == true){
                    Mix_HaltMusic();
                    checkBossTheme=false;
                }
        if(Mix_PlayingMusic() == 0)       {
            Mix_PlayMusic(bossTheme,0);
        }
        HPBoss[checkBoss/5].render(SCREEN_WIDTH -HPBoss[checkBoss/5].getWidth(), 0,NULL);
        mainBoss.render();
         for(int i =0;i < coli[3].size();i++){
        if(coli[3][i] >0){
        explosion[0].render(coliX[3][i],coliY[3][i],NULL);
        coli[3][i]--;
        }
        if(coli[3][i] <=0){
             coli[3].erase(coli[3].begin());
             coliX[3].erase(coliX[3].begin());
             coliY[3].erase(coliY[3].begin());
        }
        }
        if(checkCollision(mainship.getMainRect(),mainBoss.getBossBulletRect() )){
             Mix_PlayChannel(4,VuNoVaCham,0);

            checkMainShipCollision =true;

      }

        while(checkBoss >=25){

                Mix_HaltMusic();

                  for(int a=0;a<=3;a++){
                    Mix_PlayChannel(4,VuNoVaCham,0);

                    for(int b=0;b<=29;b++){
                            for(int k =0;k <enemyship.size();k++){
                    enemyship.at(k).eVel=0;
                    }
        background.render(scrollingOffSet,0,NULL);
        background.render(scrollingOffSet + background.getWidth(),0,NULL);
        for(int i =0; i<= 7;i++){
        BGLAYERR[i].render(0,0,NULL);
         }
        bgLayer.render(scrollingOffSet1,0,NULL);
        bgLayer.render(scrollingOffSet1 + background.getWidth(),0,NULL);
        mainShip.render(mainship.getPosX(),mainship.getPosY(),NULL);
        if(a<3){
                 for(int k =0;k <enemyship.size();k++){
                    enemyship.at(k).render();
                    }

        Boss.render(mainBoss.getbossRect().x, mainBoss.getbossRect().y,NULL);}
        bossExplosion[b/5].render(mainBoss.getbossRect().x , mainBoss.getbossRect().y,NULL);
        for(int i=0;i<enemyship.size();i++){
        explosion[b/5].render(enemyship[i].getEnemyRect().x,enemyship[i].getEnemyRect().y,NULL);}

        SDL_RenderPresent(gRenderer);
                    }
            }
             Mix_PlayChannel(7,gameWin,0);
            wingamebg.render((SCREEN_WIDTH- wingamebg.getWidth()) /2,(SCREEN_HEIGHT-wingamebg.getHeight()) /2,NULL);
            PressSpace.render((SCREEN_WIDTH- wingamebg.getWidth()) /2,(SCREEN_HEIGHT-wingamebg.getHeight()) /2+wingamebg.getHeight(),NULL);
            PressEsc.render((SCREEN_WIDTH- wingamebg.getWidth()) /2,(SCREEN_HEIGHT-wingamebg.getHeight()) /2+wingamebg.getHeight() +PressSpace.getHeight(),NULL);
            SDL_RenderPresent(gRenderer);
            checkBoss=0;
            quit =true;

        }

        }
    while(checkMainShipCollision ){
            Mix_HaltMusic();

            for(int a=0;a<=3;a++){
                    Mix_PlayChannel(4,VuNoVaCham,0);

                    for(int b=0;b<=29;b++){
                              for(int k =0;k <enemyship.size();k++){
                                enemyship.at(k).eVel=0;
                    }

        background.render(scrollingOffSet,0,NULL);
        background.render(scrollingOffSet + background.getWidth(),0,NULL);
        for(int i =0; i<= 7;i++){
        BGLAYERR[i].render(0,0,NULL);
         }
        bgLayer.render(scrollingOffSet1,0,NULL);
        bgLayer.render(scrollingOffSet1 + background.getWidth(),0,NULL);
        for(int k =0;k <enemyship.size();k++){
                    enemyship.at(k).render();
                    }
        if(demBackGround>=3)
        Boss.render(mainBoss.getbossRect().x, mainBoss.getbossRect().y,NULL);
        if(a<3)
        mainShip.render(mainship.getPosX(),mainship.getPosY(),NULL);
        explosion[b/5].render(mainship.getPosX()+mainShip.getWidth()/2-explosion[b/5].getWidth()/2,mainship.getPosY(),NULL);
        SDL_RenderPresent(gRenderer);
                    }
            }
            Mix_PlayChannel(7,gameLose,0);
        gameOverBackGround.render((SCREEN_WIDTH- gameOverBackGround.getWidth()) /2,(SCREEN_HEIGHT-gameOverBackGround.getHeight()) /2,NULL);
        PressSpace.render((SCREEN_WIDTH- gameOverBackGround.getWidth()) /2,(SCREEN_HEIGHT-gameOverBackGround.getHeight()) /2+gameOverBackGround.getHeight(),NULL);
        PressEsc.render((SCREEN_WIDTH- gameOverBackGround.getWidth()) /2,(SCREEN_HEIGHT-gameOverBackGround.getHeight()) /2+gameOverBackGround.getHeight() +PressSpace.getHeight(),NULL);
        SDL_RenderPresent(gRenderer);
checkMainShipCollision=false;
quit=true;
    }
    text1.render(0,0, NULL);
    HiScore.render(0,text1.getHeight(),NULL);
    pause[0].render((SCREEN_WIDTH- pause[0].getWidth())/2, 0,NULL);

  SDL_RenderPresent(gRenderer);

    }
if(diemso > highscore){
    ff.open("hiScore.txt",ios::out);
    ff<<mark2;
    ff.close();
}
while(checkAgain){

    while(SDL_PollEvent(&e) != 0){
         if(e.type == SDL_KEYDOWN){

        switch(e.key.keysym.sym){
            case SDLK_ESCAPE: checkAgain =false; again =false;; break;
            case SDLK_SPACE: checkAgain =false; again =true;quit =false ; break;

    }

     } else if(e.type == SDL_QUIT){
        checkAgain =false;
        again =false;
        checkMenu =false;
     }
    }

    SDL_RenderPresent(gRenderer);

}
Mix_HaltMusic();
Mix_HaltChannel(7);

mainship.freeMainShip();
enemyship.clear();
coli.clear();
coliX.clear();
coliY.clear();
lazer.bulletX.clear();
lazer.bulletY.clear();
lazer.bRect.clear();
}

}

return 0;
}
