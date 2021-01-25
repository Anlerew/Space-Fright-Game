#pragma once;

#include <stdlib.h>
#include "Sprite.h"

class Laser : public Sprite {
  public:
    Laser();
    Laser(float xPos, float yPos, bool player);

    virtual void idle();
};