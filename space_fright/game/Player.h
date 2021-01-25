#pragma once

#include "Sprite.h"

/** A Moving Sprite object with added functionality:
 *
 * players sprite behaves a bit differently than other sprites
 *
 * may have a shield (contains a pointer to another MovingSprite so that only
 *  the shield portion of the player model takes hit)
 */
class Player : public Sprite {

    MovingTexRect *shield;

    // Game just behaves and looks nicer if the bounds are differnt than the bulky texture
    Rect *shieldBounds;

    bool moving;
    bool debugMode;

    // Used so Player doesnt take multiple damage ticks when in contact with a persistance damage source
    bool showPlayer, invulnerable;
    int invulnerableCount;

  public:
    Player(bool);

    void draw(float z = 0) const;
    void idle();
    void resetPosition();

    bool isMoving() const;

    void setInvulnerable(bool b);
    bool isInvulnerable() const;

    bool checkShieldContains(float, float);

    // for changing the bounds of
    virtual bool checkCollision(const Rect &r);

    virtual void showBounds() const;

    ~Player();
};
