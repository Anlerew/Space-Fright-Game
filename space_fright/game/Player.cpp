#include "Player.h"
#include <iostream>

Player::Player(bool debug) : Sprite("images/player.png", 2, 1, -.1, -.6, .2, .2, 0, 0, true, player), shield(new MovingTexRect("images/shield.png", -.15, -.65, .3, .3, 0, 0, defaultID)), debugMode(debug), invulnerable(1), showPlayer(1), invulnerableCount(0) {
    shield->toggle();
    shieldBounds = new Rect(0, 0, .3, .3);
}

void Player::resetPosition() {
    setX(-.1);
    setY(-.6);
    setInvulnerable(true);
}

void Player::draw(float z) const {
    if (showPlayer) {
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glEnable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);

        glColor4f(1, 1, 1, 1);

        glTexCoord2f(left, bottom);
        glVertex3f(x, y - h, z);

        glTexCoord2f(left, top);
        glVertex3f(x, y, z);

        glTexCoord2f(right, top);
        glVertex3f(x + w, y, z);

        glTexCoord2f(right, bottom);
        glVertex3f(x + w, y - h, z);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // Displays bounds of player
    if (debugMode) {
        showBounds();
    }

    if (invulnerable) {
        shield->draw();
    }
}

void Player::idle() {
    MovingTexRect::idle();

    if (getX() < -1.0) {
        setX(1.8);
    } else if (getX() > 0.8) {
        setX(-1.0);
    }

    // make the player flash
    if (invulnerable) {
        showPlayer = !showPlayer;
        invulnerableCount++;
        // used as a timer to turn the player back to solid after it has started flashing
        if (invulnerableCount > 100) {
            invulnerable = false;
            invulnerableCount = 0;
            showPlayer = true;
        }
    }

    // used for animation
    if (dx != 0) {
        moving = true;
    } else {
        moving = false;
    }

    shield->setX(x - .05);
    shield->setY(y + .05);
    shieldBounds->setX(shield->getX());
    shieldBounds->setY(shield->getY());
}

void Player::showBounds() const {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    shieldBounds->draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool Player::isMoving() const {
    return moving;
}

void Player::setInvulnerable(bool b) {
    invulnerable = b;
}

bool Player::isInvulnerable() const {
    return invulnerable;
}

bool Player::checkShieldContains(float x, float y) {
    return shieldBounds->contains(x, y);
}

bool Player::checkCollision(const Rect &two) {
    return shieldBounds->checkCollision(two);
}

Player::~Player() {
    delete shield;
    delete shieldBounds;
}
