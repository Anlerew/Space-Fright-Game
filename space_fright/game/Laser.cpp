#include "Laser.h"

Laser::Laser() : Laser(0, 1, false) {}

Laser::Laser(float xPos, float yPos, bool player) : Sprite(player ? "images/player-laser.png" : "images/laser.png", 2, 1, xPos, yPos, .01, .06, 0, player ? 0.01 : -.01, true, player ? ID::playerLaser : ID::laser) {
    setX(xPos - getW() / 2.0);
    setY(yPos - getH() * (player ? 1 : -1) / 2.0);
}

void Laser::idle() {
    MovingTexRect::idle();
    advance();
    /* if (y < -.65) {
        y = -.65;
        dx = 0;
    } */
}
