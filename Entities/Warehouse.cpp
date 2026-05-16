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

bool Warehouse::isClicked(int x, int y) const
{
    return (x >= RefPoint.x && x <= RefPoint.x + 100 &&
            y >= RefPoint.y && y <= RefPoint.y + 80);
}

void Warehouse::openInventoryWindow() const
{
    window* pWind = pGame->getWind();
    window* pNewWind = new window(300, 200, 200, 200);
    pNewWind->SetBrush(WHITE);
    pNewWind->SetPen(BLACK, 1);
    pNewWind->DrawRectangle(0, 0, 300, 200);

    pNewWind->SetPen(BLACK, 2);
    pNewWind->SetFont(20, BOLD, BY_NAME, "Arial");
    pNewWind->DrawString(80, 20, "INVENTORY");

    pNewWind->SetFont(16, BOLD, BY_NAME, "Arial");
    stringstream ss;
    ss << "Eggs: " << eggsCount;
    pNewWind->DrawString(50, 60, ss.str());

    stringstream ss2;
    ss2 << "Milk: " << milkCount;
    pNewWind->DrawString(50, 100, ss2.str());

    pNewWind->SetFont(12, PLAIN, BY_NAME, "Arial");
    pNewWind->DrawString(50, 160, "Click anywhere to close");

    pNewWind->UpdateBuffer();

    int x, y;
    pNewWind->WaitMouseClick(x, y);

    delete pNewWind;
}