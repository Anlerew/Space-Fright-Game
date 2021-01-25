#include <iostream>
#include "Enemy.h"

Enemy::Enemy(bool debug) : Sprite("images/enemy.png", 2, 1, -.1, 0.9, .2, .2, 0, 0, true, enemy), shield(new MovingTexRect("images/shield.png", -.15, -.65, .3, .3, 0, 0, defaultID)), debugMode(debug) {
    shield->toggle();
    shieldBounds = new Rect(0, 0, .3, .3);
}

void Enemy::resetPosition() {
    setX((rand() % 200 - 100 ) / 100.0);
    setY(0.9);
}

void Enemy::idle() {
    MovingTexRect::idle();

    if (getX() < -1.0) {
        setX(1.8);
    } else if (getX() > 0.8) {
        setX(-1.0);
    }
    dx = (rand() % 200 - 100) / (getX() * 10000.0);

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

void Enemy::showBounds() const {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    shieldBounds->draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool Enemy::isMoving() const {
    return moving;
}

bool Enemy::checkCollision(const Rect &two) {
    return shieldBounds->checkCollision(two);
}

Enemy::~Enemy() {
    delete shield;
    delete shieldBounds;
}