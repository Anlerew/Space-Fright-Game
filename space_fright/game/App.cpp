#include <iostream>
#include <string>
#include <time.h>
#include "App.h"

#define ENABLE_DEBUG false

static App *singleton;
const float App::PLAYER_BASE_SPEED = .02;   // Cover 1% of width per move
static int framesCount = 0;

App::App(int argc, char** argv, int width, int height, const char* title): GlutApp(argc, argv, width, height, title){
    //when a key is pressed, the key only triggers one event
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    srand(time(NULL));
    debugModeEnabled = ENABLE_DEBUG;
    preGame = true;
    paused = false;
    gameOver = false;
    difficulty = 0;
    seconds = 0;
    showExplosion = false;

    hud = new HUD();
    staticShapes.push_back(hud);

    infoScreen = new TexRect("images/game-info.png", -1, 1, 2, 2);
    staticShapes.push_back(infoScreen);
    // for animation of the laser on the infoscreen
    // generateObject(demo);
    bg = new TexRect("images/game-background.png", -1, 1, 2, 2);
    staticShapes.push_back(bg);

    pauseScreen = new TexRect("images/game-paused.png", -1, 1, 2, 2);
    staticShapes.push_back(pauseScreen);

    lossScreen = new TexRect("images/game-lost.png", -1, 1, 2, 2);
    staticShapes.push_back(lossScreen);

    player = new Player(debugModeEnabled);
    animatedSprites.push_back(player);

    enemy = new Enemy(debugModeEnabled);
    animatedSprites.push_back(enemy);

    explosion = new Sprite("images/explosion.png", 5, 5, -0.8, 0.8, 0.4, 0.4, false, ID::explosion);
    staticShapes.push_back(explosion);

    singleton = this;

    singleton->gameLoop(0);
    singleton->generateObjectLoop(1);
    singleton->displayFPS(2);
    singleton->animatePlayer(3);
} 

void App::draw() const {
    if (gameOver) {
        lossScreen->draw();
    } else {
        if (preGame) {
            infoScreen->draw();
        } else if (paused) {
            pauseScreen->draw();
        } else {
            bg->draw();
        }
    }
    if (!preGame) {
        for (auto i = animatedSprites.begin(); i != animatedSprites.end(); i++) {
            (*i)->draw();
            if (debugModeEnabled)
                (*i)->showBounds();
        }
        if (showExplosion) {
            explosion->draw();
        }

        hud->draw();
    }
}

void App::generateFallingObject() {
    //We will generate only laser at difficulty 0, rock at difficuly 1, double laser at difficulty 2, energy at difficulty 3, and shield at difficulty 4 and so on
    int random = rand() % (1 + difficulty);
    int random2 = rand() % (1 + random / 2);

    if (debugModeEnabled) {
        std::cout << 'Difficulty: ' << difficulty << ' Random: ' << random2 << std::endl;
    }

    switch(random2) {
      case 0:
        generateObject(laser); break;
      case 1:
        generateObject(rock); break;
      case 2:
        generateObject(doubleLaser); break;
      case 3:
        generateObject(energy); break;
      case 4:
        generateObject(shield); break;
      default:
        generateObject(laser); break;
    }
}

void App::generateObject(ID id) {
    float posX = ((rand() % 300) - 150) / 150.0;    // between -1 to 1
    float ex = enemy->getX() + enemy->getW() / 2.0;
    float ey = enemy->getY() - enemy->getH();
    float dx = ((rand() % 20) - 10) / -10000.0;

    if (debugModeEnabled) {
        std::cout << "ID: " << id << std::endl;
    }

    switch (id) {
      case laser:
        animatedSprites.push_back(new Laser(ex, ey, false));
        break;

      case rock:
        animatedSprites.push_back(new Sprite("images/rock.png", posX, 1.0, .15, .15, dx, -.01, rock));
        break;

      case doubleLaser:
        animatedSprites.push_back(new Laser(ex - 0.02, ey, false));
        animatedSprites.push_back(new Laser(ex + 0.02, ey, false));
        break;

      case energy:
        animatedSprites.push_back(new Sprite("images/energy.png", posX, 1.0, .06, .1, 0, -.01, energy));
        break;

      case shield:
        animatedSprites.push_back(new Sprite("images/shield-gain.png", posX, 1.0, .1, .1, 0, -.01, shield));
        break;
    }
}

void App::idle(){
}

void App::keyDown(unsigned char key, float x, float y){
    if (key == ' ') {
        if (preGame) {
            preGame = false;
            seconds = 0;
        } else {
            paused = !paused;
            glutPostRedisplay();
        }
    }
}

void App::keyUp(unsigned char key, float x, float y){
    if (key == 27){
        exit(0);
    }
}

void App::specialKeyDown(int key, float x, float y){
    float randomFloat = (rand() % 100) / 100.0;

    switch(key) {
      case GLUT_KEY_UP:
        if (randomFloat < singleton->hud->getEnergy()) {
            float px = player->getX() + player->getW() / 2.0;
            float py = player->getY();
            animatedSprites.push_back(new Laser(px, py, true));
        }
        break;

      case GLUT_KEY_LEFT:
        player->setDX(player->getDX() - PLAYER_BASE_SPEED);
        break;

      case GLUT_KEY_RIGHT:
        player->setDX(player->getDX() + PLAYER_BASE_SPEED);
        break;
    }
}

void App::specialKeyUp(int key, float x, float y){
    switch(key) {
      case GLUT_KEY_LEFT:
        player->setDX(player->getDX() + PLAYER_BASE_SPEED);
        break;
      case GLUT_KEY_RIGHT:
        player->setDX(player->getDX() - PLAYER_BASE_SPEED);
        break;
    }
}

void App::displayFPS(int id) {
    if (singleton->debugModeEnabled) {
        std::cout << "FPS: " << framesCount << std::endl;
    }

    framesCount = 0;
    if (!singleton->paused && !singleton->gameOver && !singleton->preGame) {
        singleton->seconds++;
        if (singleton->seconds % App::DIFFICULTY_INCREASE_MODIFIER == 0) {
            singleton->difficulty++;
            std::cout << "Difficulty " << singleton->difficulty << std::endl;
        }
    }
    glutTimerFunc(1000, singleton->displayFPS, id);
}

void App::explosionAnimation(int id) {
    singleton->explosion->advance();

    if (!singleton->explosion->isDone()) {
        glutTimerFunc(25, singleton->explosionAnimation, id);
    } else {
        singleton->showExplosion = false;
        singleton->explosion->reset();
    }
}

void App::gameLoop(int id) {
    bool playerStruck = false;
    bool enemyStruck = false;
    if (!singleton->paused && !singleton->gameOver) {
        for (auto i = singleton->animatedSprites.begin(); i != singleton->animatedSprites.end();) {
            bool removeSprite = false;

            (*i)->idle();

            switch ((*i)->getID()) {
              case shield:
                if ((*i)->getY() < -0.8) { // shield left the screen
                    removeSprite = true;
                } else if (singleton->player->checkCollision(*(*i))) {
                    singleton->player->setInvulnerable(true);
                    removeSprite = true;
                }
                break;

              case energy:
                if ((*i)->getY() < -0.8) { // energy left the screen
                    removeSprite = true;
                } else if (singleton->player->checkCollision(*(*i))) {
                    singleton->hud->increaseEnergy();
                    removeSprite = true;
                }
                break;

              case rock:
                if ((*i)->getY() < -0.8 || (*i)->getX() < -1.2 || (*i)->getX() > 1.0 ) { // rock left the screen
                    removeSprite = true;
                } else if (singleton->player->checkCollision(*(*i))) {
                    playerStruck = true;
                    removeSprite = true;
                }
                break;

              case laser:
                if ((*i)->getY() < -0.8) { // laser left play area
                    removeSprite = true;
                } else if (singleton->player->checkCollision(*(*i)) && !singleton->player->isInvulnerable()) {
                    singleton->player->setInvulnerable(true);
                    singleton->hud->decreaseEnergy();
                    if (! singleton->hud->hasEnergy()) {
                        playerStruck = true;
                    }
                    removeSprite = true;
                } else {
                    (*i)->advance();
                }
                break;

              case playerLaser:
                if ((*i)->getY() > 1.0 ) { // player Laser left the screen
                    removeSprite = true;
                } else if (singleton->enemy->checkCollision(*(*i))) {
                    singleton->hud->increaseScore(1);
                    enemyStruck = true;
                    removeSprite = true;
                } else {
                    (*i)->advance();
                }
                break;
            }

            if (removeSprite) {
                delete (*i);
                i = singleton->animatedSprites.erase(i);
            } else {
                ++i;
            }
        }

        if (playerStruck) {
            singleton->explosion->setX(singleton->player->getX());
            singleton->explosion->setY(singleton->player->getY());
            singleton->showExplosion = true;
            explosionAnimation(4);
            singleton->hud->decreaseHealth();
            if (singleton->hud->healthIsEmpty()) {
                singleton->hud->setSeconds(singleton->seconds);
                singleton->gameOver = true;
            } else {
                singleton->player->resetPosition();
            }
        } else if (enemyStruck) {
            singleton->explosion->setX(singleton->enemy->getX());
            singleton->explosion->setY(singleton->enemy->getY());
            singleton->showExplosion = true;
            singleton->enemy->resetPosition();
            explosionAnimation(4);
        } else {
            singleton->showExplosion = false;
        }

        glutPostRedisplay();
        framesCount++;
    }
    glutTimerFunc(1000.0 / 60, singleton->gameLoop, id);
}

void App::animatePlayer(int id) {
    if (!singleton->paused && !singleton->gameOver) {
        singleton->player->advance();
        singleton->enemy->advance();
    }

    glutTimerFunc(100, singleton->animatePlayer, id);
}

void App::generateObjectLoop(int id) {
    if (!singleton->paused && !singleton->gameOver && !singleton->preGame)
        singleton->generateFallingObject();

    glutTimerFunc(500, singleton->generateObjectLoop, id);
}

App::~App(){
    for (auto i = animatedSprites.begin(); i != animatedSprites.end(); i++) {
        delete *i;
    }

    for (auto i = staticShapes.begin(); i != staticShapes.end(); i++) {
        delete *i;
    }
    std::cout << "Exiting..." << std::endl;
}
