#pragma once
#include "Core/GameObject.h"
#include "Entities/Animal.h"

class GrassFood : public GameObject {
private:
    int foodAmount;
   

public:
    Animal* lastAnimal;
    int collisionCooldown;//task 31
    bool markedForRemoval; 
    GrassFood(Game* pGame, point ref);

    virtual void draw() const override;
    void collisionAction(Animal* pAnimal);

    int  getX()     const { return RefPoint.x; }
    int  getY()     const { return RefPoint.y; }
    bool isMarked() const { return markedForRemoval; }
};