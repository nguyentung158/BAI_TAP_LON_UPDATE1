
class mouse{
public:
    mouse();
    ~mouse();
    void setPoisition(int x, int y);
    void handleEvent(SDL_Event &e, bool &again, bool &checkMenu);
    void render();
private:
    SDL_Point Position;
    int CurrentStart ;
    int CurrentQuit ;
};
