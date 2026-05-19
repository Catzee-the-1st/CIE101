#include "BudgetBar.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

// Static counters
int ChickIcon::chick_count = 0;
int cowIcon::cow_count = 0;

BudgetbarIcon::BudgetbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Drawable(r_pGame, r_point, r_width, r_height)
{
    image_path = img_path;
}

void BudgetbarIcon::draw() const
{
    window* pWind = pGame->getWind();
    pWind->DrawImage(image_path, RefPoint.x + 5, RefPoint.y, width, height);
}

ChickIcon::ChickIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
    
}

void ChickIcon::onClick()
{
    cout << "ChickIcon clicked" << endl;

    if (pGame->budget >= 100) {
        pGame->budget -= 100;

        point p;
        mt19937 gen1(random_device{}());
        mt19937 gen2(random_device{}());
        p.x = uniform_int_distribution<int>(range_min_x, range_max_x)(gen1);
        p.y = uniform_int_distribution<int>(range_min_y, range_max_y)(gen2);

        if (chick_count < 150) {
            pGame->chicken_list[chick_count] = new Chick(pGame, p, 50, 50, "images\\chick.jpg");
            
            chick_count++;
        }
        else {
            cout << "Maximum chicks reached!" << endl;
        }

        pGame->clearBudget();
        string budget_string = "BUDGET = $" + to_string(pGame->budget);
        string water_string = "WATER BOTTLES = " + to_string(pGame->water_counter);
        pGame->printBudget(water_string + "     " + budget_string);
    }
    else {
        pGame->printMessage("Not enough budget to buy a chick! (need $100)");
    }
}

cowIcon::cowIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
    
}

void cowIcon::onClick()
{
    cout << "cowIcon clicked" << endl;

    if (pGame->budget >= 150) {
        pGame->budget -= 150;

        point p;
        mt19937 gen1(random_device{}());
        mt19937 gen2(random_device{}());
        p.x = uniform_int_distribution<int>(range_min_x, range_max_x)(gen1) + 15;
        p.y = uniform_int_distribution<int>(range_min_y, range_max_y)(gen2);

        if (cow_count < 150) {
            pGame->cow_list[cow_count] = new Cow(pGame, p, 50, 50, "images\\cow.jpg");
            cow_count++;
        }
        else {
            cout << "Maximum cows reached!" << endl;
        }

        pGame->clearBudget();
        string budget_string = "BUDGET = $" + to_string(pGame->budget);
        string water_string = "WATER BOTTLES = " + to_string(pGame->water_counter);
        pGame->printBudget(water_string + "     " + budget_string);
    }
    else {
        pGame->printMessage("Not enough budget to buy a cow! (need $150)");
    }
}

WaterIcon::WaterIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void WaterIcon::onClick()
{
    cout << "WaterIcon clicked" << endl;

    if (pGame->budget >= 10) {
        pGame->budget -= 10;
        pGame->increasewatercounter();
        pGame->spawnGreenArea();

        pGame->clearBudget();
        string budget_string = "BUDGET = $" + to_string(pGame->budget);
        string water_string = "WATER BOTTLES = " + to_string(pGame->water_counter);
        pGame->printBudget(water_string + "     " + budget_string);
    }
    else {
        pGame->printMessage("Not enough budget to buy water! (need $10)");
    }
}

Budgetbar::Budgetbar(Game* r_pGame, point r_point, int r_width, int r_height)
    : Drawable(r_pGame, r_point, r_width, r_height)
{
    iconsImages[ICON_CHICK] = "images\\chick.jpg";
    iconsImages[Cow_icon] = "images\\cow.jpg";
    iconsImages[ICON_WATER] = "images\\water.jpg";

    const int gap = 30;
    point p = { gap, config.toolBarHeight };

    iconsList = new BudgetbarIcon * [BUDGET_ITEM_COUNT];
    for (int i = 0; i < BUDGET_ITEM_COUNT; i++)
        iconsList[i] = nullptr;

    iconsList[ICON_CHICK] = new ChickIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_CHICK]);
    p.x += config.iconWidth + gap;

    iconsList[Cow_icon] = new cowIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[Cow_icon]);
    p.x += config.iconWidth + gap;

    iconsList[ICON_WATER] = new WaterIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_WATER]);
}

Budgetbar::~Budgetbar()
{
    for (int i = 0; i < BUDGET_ITEM_COUNT; i++)
        delete iconsList[i];
    delete[] iconsList;
}

void Budgetbar::draw() const
{
    for (int i = 0; i < BUDGET_ITEM_COUNT; i++) {
        if (iconsList[i] != nullptr)
            iconsList[i]->draw();
    }

    window* pWind = pGame->getWind();
    pWind->SetFont(16, BOLD, BY_NAME, "Arial");
    pWind->DrawString((2 * config.toolBarHeight) - 100, (2 * config.toolbarItemWidth) - 10, "100$");
    pWind->DrawString((2 * config.toolBarHeight) + 5, (2 * config.toolbarItemWidth) - 10, "150$");
    pWind->DrawString((2 * config.toolBarHeight) + 110, (2 * config.toolbarItemWidth) - 10, "10$");

    pWind->SetPen(BLACK, 3);
    pWind->DrawLine(0, 2 * config.toolBarHeight, pWind->GetWidth(), 2 * config.toolBarHeight);
}

bool Budgetbar::handleClick(int x, int y)
{
    const int gap = 30;
    const int totalWidth = config.iconWidth + gap;

    if (x > BUDGET_ITEM_COUNT * totalWidth)
        return false;

    int clickedIconIndex = x / totalWidth;
    int offsetInsideBlock = x % totalWidth;

    if (clickedIconIndex >= BUDGET_ITEM_COUNT)
        return false;

    if (offsetInsideBlock >= gap && iconsList[clickedIconIndex] != nullptr)
        iconsList[clickedIconIndex]->onClick();

    return false;
}