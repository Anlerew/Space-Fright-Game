#pragma once

#include "Sprite.h"

class Enemy : public Sprite {
    MovingTexRect *shield;

    // Game just behaves and looks nicer if the bounds are differnt than the bulky texture
    Rect *shieldBounds;

    bool moving;
    bool debugMode;

  public:
    Enemy(bool);

    void idle();
    bool isMoving() const;
    void resetPosition();
    
    // for changing the bounds of
    virtual bool checkCollision(const Rect &r);

    virtual void showBounds() const;

    ~Enemy();
};
