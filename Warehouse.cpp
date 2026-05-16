#include "Warehouse.h"
#include "../Core/Game.h"
#include "../Config/GameConfig.h"
#include <sstream>
using namespace std;

Warehouse::Warehouse(Game* pGame, point ref)
    : Drawable(pGame, ref, 100, 80)
{
    eggsCount = 0;
    milkCount = 0;
}

void Warehouse::draw() const
{
    window* pWind = pGame->getWind();

    pWind->SetPen(BLACK, 2);
    pWind->SetBrush(BROWN);
    pWind->DrawRectangle(RefPoint.x, RefPoint.y,
        RefPoint.x + 100, RefPoint.y + 80);

    pWind->SetBrush(DARKRED);
    pWind->DrawRectangle(RefPoint.x, RefPoint.y,
        RefPoint.x + 100, RefPoint.y + 20);

    pWind->SetPen(WHITE, 1);
    pWind->SetFont(12, BOLD, BY_NAME, "Arial");
    pWind->DrawString(RefPoint.x + 5, RefPoint.y + 2, "WAREHOUSE");

    pWind->SetPen(BLACK, 1);
    pWind->SetFont(13, BOLD, BY_NAME, "Arial");
    stringstream ss;
    ss << "Eggs: " << eggsCount;
    pWind->DrawString(RefPoint.x + 5, RefPoint.y + 30, ss.str());

    stringstream ss2;
    ss2 << "Milk: " << milkCount;
    pWind->DrawString(RefPoint.x + 5, RefPoint.y + 55, ss2.str());
}