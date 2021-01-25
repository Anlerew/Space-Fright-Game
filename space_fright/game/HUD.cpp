#include "HUD.h"

HUD::HUD(){
    score = 0;

    scoreText = new Text(-.15, -.85, "Score: 0", 1, 1, 1);
    hudComponents.push_back(scoreText);

    timerText = new Text(1, .5, "", 1, 1, 1);
    hudComponents.push_back(timerText);

    healthBar = new Bar("images/health-full.png", -.99, -.85, .55, .1, "images/health-empty.png", 1);
    hudComponents.push_back(healthBar);

    energyBar = new Bar("images/energy-full.png", .54, -.85, .45, .1, "images/energy-empty.png", 1);
    hudComponents.push_back(energyBar);
}

HUD::HUD(HUD &other) {
    score = other.score;

    scoreText = new Text(.3, -.8, "Score: 0", 1, 1, 1);
    hudComponents.push_back(scoreText);

    timerText = new Text(1, .5, "", 1, 1, 1);
    hudComponents.push_back(timerText);

    healthBar = new Bar("images/health-full.png", -.99, -.85, .55, .125, "images/health-empty.png", 1);
    hudComponents.push_back(healthBar);

    energyBar = new Bar("images/energy-full.png", -.35, -.85, .55, .125, "images/energy-empty.png", 1);
    hudComponents.push_back(energyBar);
}

void HUD::increaseScore(int amount) {
    score += amount;
    scoreText->setText("Score: " + std::to_string(score));
}

void HUD::increaseHealth() {
    healthBar->increment(1.0 / TOTAL_HEALTH_POINTS);
}

void HUD::decreaseHealth() {
    healthBar->increment(-1.0 / TOTAL_HEALTH_POINTS);
    replenishEnergy();
}

void HUD::increaseEnergy() {
    energyBar->increment(.25);
}

void HUD::decreaseEnergy() {
    energyBar->increment(-.1);
}

void HUD::replenishEnergy() {
    energyBar->increment(1);
}

bool HUD::hasEnergy() const {
    return !energyBar->isEmpty();
}

float HUD::getEnergy() const {
    return energyBar->getPercent();
}

void HUD::draw(float z) const {
    for (Rect *t : hudComponents) {
        t->draw(z);
    }
}

bool HUD::healthIsEmpty() {
    if (healthBar->isEmpty()) {
        lose();
    }
    return healthBar->isEmpty();
}

void HUD::setSeconds(int s) {
    timerText->setText("Great work, you lived for " + std::to_string(s) + " seconds");
}

void HUD::lose() {
    timerText->setX(-.6);

    scoreText->setX(-.3);
    scoreText->setY(.4);
    scoreText->setColor(1, 1, 1);
    scoreText->setText("and hit " + std::to_string(score) + " enemies!");
}

HUD::~HUD() {
    for (Rect *t : hudComponents) {
        delete t;
    }
}
