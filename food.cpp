#include "food.h"
#include "Core/Game.h"
#include "Entities/Animal.h"
#include <sstream>
using namespace std;

GrassFood::GrassFood(Game* pGame, point ref)
    : GameObject(pGame, ref, 50, 50, GREEN, GREEN)
{
    foodAmount = 10;
    collisionCooldown = 0;
    markedForRemoval = false;
}

void GrassFood::draw() const
{
    if (markedForRemoval) return;

    window* pWind = pGame->getWind();

    pWind->SetPen(DARKGREEN, 2);
    pWind->SetBrush(GREEN);
    pWind->DrawRectangle(RefPoint.x, RefPoint.y,
        RefPoint.x + 50, RefPoint.y + 50);

    stringstream ss;
    ss << foodAmount;
    pWind->SetPen(BLACK, 1);
    pWind->SetFont(16, BOLD, BY_NAME, "Arial");
    pWind->DrawString(RefPoint.x + 15, RefPoint.y - 20, ss.str());
}

void GrassFood::collisionAction(Animal* pAnimal)
{
    if (pAnimal == nullptr) return;
    if (markedForRemoval) return;

    if (collisionCooldown > 0) {
        collisionCooldown--;
        return;
    }

    collisionCooldown = 50;
    foodAmount--;

    Chick* pChick = dynamic_cast<Chick*>(pAnimal);
    if (pChick != nullptr)
        pChick->increaseCounter();

    Cow* pCow = dynamic_cast<Cow*>(pAnimal);
    if (pCow != nullptr)
        pCow->increaseCounter();

    if (foodAmount <= 0)
        markedForRemoval = true;
}