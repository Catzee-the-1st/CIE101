#pragma once
#include "../Core/Drawable.h"

class Animal : public Drawable
{
private:
    string image_path;
public:
    point curr_pos;
    point curr_vel;
    Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void draw() const override;
    virtual void moveStep() = 0;
};

class Chick : public Animal
{
private:
    int speed = 2;
    point target;
    bool has_target = false;
    int eatCounter = 0;
    int eggCount = 0;
    int productionTimer = 0;
    static const int eggProductionTime = 500;

public:
    static int next_id;
    int id;
    Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void moveStep();
    virtual void draw() const override;

    void updateProduction() {
        productionTimer++;
        if (productionTimer >= eggProductionTime) {
            productionTimer = 0;
            eggCount++;
        }
    }
    void increaseCounter() {
        eatCounter++;
        if (eatCounter >= 10) {
            eatCounter = 0;
            eggCount++;
        }
    }
    int getEatCounter() const { return eatCounter; }
    int getEggCount()   const { return eggCount; }
    int collectEggs() { int eggs = eggCount; eggCount = 0; return eggs; }
};

class Cow : public Animal
{
private:
    int speed = 1;
    point target;
    bool has_target = false;
    int eatCounter = 0;
    int milkCount = 0;
    int productionTimer = 0;
    static const int milkProductionTime = 750;

public:
    static int next_id;
    int id;
    Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void moveStep();
    virtual void draw() const override;

    void updateProduction() {
        productionTimer++;
        if (productionTimer >= milkProductionTime) {
            productionTimer = 0;
            milkCount++;
        }
    }
    void increaseCounter() {
        eatCounter++;
        if (eatCounter >= 15) {
            eatCounter = 0;
            milkCount++;
        }
    }
    int getEatCounter() const { return eatCounter; }
    int getMilkCount() const { return milkCount; }
    int collectMilk() { int milk = milkCount; milkCount = 0; return milk; }
};

class Wolf : public Animal
{
private:
    point target;
    bool has_target = false;
    int clickCount = 0;
    static const int clicksNeeded = 5;
public:
    static float speed;
    static int next_id;
    int id;
    bool isDead;
    Wolf(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void moveStep() override;
    void move();
    virtual void draw() const override;
    bool isClicked(int clickX, int clickY) const;
    bool hit() { clickCount++; return clickCount >= clicksNeeded; }
    int getClickCount() const { return clickCount; }
};