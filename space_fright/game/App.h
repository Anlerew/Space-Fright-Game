#pragma once

#include "GlutApp.h"
#include "HUD.h"
#include "Player.h"
#include "Enemy.h"
#include "Laser.h"
#include <vector>


class App: public GlutApp {
    // base duration per frame
    static const float PLAYER_BASE_SPEED;

    // how often in seconds to ramp up the Difficulty
    static const int DIFFICULTY_INCREASE_MODIFIER = 30;

    HUD *hud;

    std::vector<Sprite *> animatedSprites;
    std::vector<Shape *> staticShapes;

    TexRect *infoScreen;
    TexRect *bg;
    TexRect *pauseScreen;
    TexRect *lossScreen;

    Player *player;
    Enemy *enemy;
    Sprite *explosion;

    bool preGame;
    bool gameOver;
    bool showExplosion;
    bool paused;
    int seconds;
    // increases as game goes on. Controls laser, rock, and energy generateObject rate
    int difficulty;
    // Whether or not to turn on certain debugging features
    bool debugModeEnabled;
public:
    
    App(int argc, char** argv, int width, int height, const char* title);
    
    void draw() const;

    void keyDown(unsigned char key, float x, float y);
    void keyUp(unsigned char key, float x, float y);
    void specialKeyDown(int key, float x, float y);
    void specialKeyUp(int key, float x, float y);
    void idle();

    // controls spawnRates of the specific objects
    void generateFallingObject();
    //Spawns each individual falling object
    void generateObject(ID id);

    static void displayFPS(int id);
    static void gameLoop(int id);
    static void generateObjectLoop(int id);
    static void animatePlayer(int id);
    static void explosionAnimation(int id);

    ~App();
};
