#pragma once
#include "Core/GameObject.h"
#include "Entities/Animal.h"

class GrassFood : public GameObject {
private:
    int foodAmount;
    int collisionCooldown;
    bool markedForRemoval;

public:
    GrassFood(Game* pGame, point ref);

    virtual void draw() const override;
    void collisionAction(Animal* pAnimal);

    int  getX()     const { return RefPoint.x; }
    int  getY()     const { return RefPoint.y; }
    bool isMarked() const { return markedForRemoval; }
};