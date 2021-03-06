#pragma once

#include "Bar.h"
#include "Text.h"
#include <vector>

// Displays all non moving HUD components
class HUD : public Shape{
    static const int TOTAL_HEALTH_POINTS = 5;
    Text *scoreText;
    Text *timerText;

    Bar *healthBar;
    Bar *energyBar;

    std::vector<Rect *> hudComponents;

    int score;

  public:
    HUD();
    HUD(HUD &other);

    void increaseScore(int amount);

    void setSeconds(int s);

    void increaseHealth();
    void decreaseHealth();
    bool healthIsEmpty();

    void increaseEnergy();
    void decreaseEnergy();
    void replenishEnergy();
    bool hasEnergy() const;
    float getEnergy() const;

    void lose();

    void draw(float z = 0) const;

    ~HUD();
};
