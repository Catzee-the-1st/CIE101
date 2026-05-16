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

bool Warehouse::sellEggs(int count)
{
    if (eggsCount >= count && count > 0) {
        eggsCount = eggsCount - count;
        int earned = count * 5;
        pGame->budget = pGame->budget + earned;
        return true;
    }
    return false;
}

bool Warehouse::sellMilk(int count)
{
    if (milkCount >= count && count > 0) {
        milkCount = milkCount - count;
        int earned = count * 10;
        pGame->budget = pGame->budget + earned;
        return true;
    }
    return false;
}

void Warehouse::openInventoryWindow()
{
    window* pNewWind = new window(300, 250, 200, 200);

    const int eggBtnX = 200, eggBtnY = 55;
    const int milkBtnX = 200, milkBtnY = 95;
    const int eggPrice = 5;
    const int milkPrice = 10;

    int currentEggs = eggsCount;
    int currentMilk = milkCount;

    while (true) {
        pNewWind->SetBrush(WHITE);
        pNewWind->SetPen(BLACK, 1);
        pNewWind->DrawRectangle(0, 0, 300, 250);

        pNewWind->SetPen(BLACK, 2);
        pNewWind->SetFont(20, BOLD, BY_NAME, "Arial");
        pNewWind->DrawString(80, 10, "INVENTORY");

        pNewWind->SetFont(14, BOLD, BY_NAME, "Arial");

        stringstream ss;
        ss << "Eggs: " << currentEggs;
        pNewWind->DrawString(30, 60, ss.str());

        pNewWind->SetBrush(LIGHTGRAY);
        pNewWind->SetPen(BLACK, 1);
        pNewWind->DrawRectangle(eggBtnX, eggBtnY, eggBtnX + 60, eggBtnY + 20);
        pNewWind->SetFont(10, BOLD, BY_NAME, "Arial");
        pNewWind->DrawString(eggBtnX + 5, eggBtnY + 5, "SELL ($5)");

        stringstream ss2;
        ss2 << "Milk: " << currentMilk;
        pNewWind->SetBrush(WHITE);
        pNewWind->SetPen(BLACK, 1);
        pNewWind->DrawString(30, 100, ss2.str());

        pNewWind->SetBrush(LIGHTGRAY);
        pNewWind->DrawRectangle(milkBtnX, milkBtnY, milkBtnX + 60, milkBtnY + 20);
        pNewWind->SetFont(10, BOLD, BY_NAME, "Arial");
        pNewWind->DrawString(milkBtnX + 5, milkBtnY + 5, "SELL ($10)");

        pNewWind->SetFont(12, PLAIN, BY_NAME, "Arial");
        pNewWind->SetBrush(WHITE);
        pNewWind->DrawString(80, 220, "Click X to close");

        pNewWind->UpdateBuffer();

        int x, y;
        clicktype clk = pNewWind->GetMouseClick(x, y);

        if (clk == NO_CLICK) {
            continue;
        }

        if (x >= eggBtnX && x <= eggBtnX + 60 && y >= eggBtnY && y <= eggBtnY + 20) {
            if (currentEggs > 0) {
                currentEggs--;
                pGame->budget = pGame->budget + eggPrice;
            }
        }
        else if (x >= milkBtnX && x <= milkBtnX + 60 && y >= milkBtnY && y <= milkBtnY + 20) {
            if (currentMilk > 0) {
                currentMilk--;
                pGame->budget = pGame->budget + milkPrice;
            }
        }
        else if (x >= 270 && x <= 295 && y >= 5 && y <= 25) {
            break;
        }
    }

    eggsCount = currentEggs;
    milkCount = currentMilk;

    delete pNewWind;
}