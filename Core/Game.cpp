#include "Game.h"
#include "../Config/GameConfig.h"
#include "../UI/Toolbar.h"
#include "../UI/BudgetBar.h"
#include "../Entities/Warehouse.h"
#include "..\food.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;

Game::Game()
{
    PlaySound(TEXT("images\\sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    isPaused = false;
    for (int i = 0; i < 50; i++) {
        chicken_list[i] = nullptr;
        cow_list[i] = nullptr;
        food_list[i] = nullptr;
        egg_items[i].active = false;
        milk_items[i].active = false;
    }
    for (int i = 0; i < 10; i++)
        wolves[i] = nullptr;

    green_count = 0;
    srand((unsigned int)time(0));

    pWind = CreateWind(config.windWidth, config.windHeight, config.wx, config.wy);
    pWind->SetBuffering(true);
    createToolbar();
    createBudgetbar();

    point warehousePos = { config.windWidth - 120, config.windHeight - config.statusBarHeight - 90 };
    warehouse = new Warehouse(this, warehousePos);

    clearStatusBar();
}

void Game::setPaused(bool p) {
    isPaused = p;
}

bool Game::getPaused() const {
    return isPaused;
}

Game::~Game()
{
    for (int i = 0; i < 50; i++) {
        delete chicken_list[i];
        delete cow_list[i];
        delete food_list[i];
    }
    for (int i = 0; i < 10; i++)
        delete wolves[i];
    delete gameToolbar;
    delete gameBudgetbar;
    delete warehouse;
    delete pWind;
}

void Game::addFood(GrassFood* pFood)
{
    for (int i = 0; i < 50; i++) {
        if (food_list[i] == nullptr) {
            food_list[i] = pFood;
            return;
        }
    }
    delete pFood;
}

void Game::removeGameObject(GrassFood* pFood)
{
    for (int i = 0; i < 50; i++) {
        if (food_list[i] == pFood) {
            delete food_list[i];
            food_list[i] = nullptr;
            return;
        }
    }
}

void Game::spawnGreenArea()
{
    if (green_count >= 20) return;
    int min_x = 0;
    int max_x = config.windWidth - 50;
    int min_y = 2 * config.toolBarHeight;
    int max_y = config.windHeight - config.statusBarHeight - 50;

    point p;
    p.x = rand() % (max_x - min_x + 1) + min_x;
    p.y = rand() % (max_y - min_y + 1) + min_y;

    GrassFood* pFood = new GrassFood(this, p);
    addFood(pFood);

    green_areas[green_count] = p;
    green_count++;
}

void Game::updateGoals()
{
    goal_chickens = 10 + (level - 1) * 5;
    goal_cows = 5 + (level - 1) * 3;
    goal_water = 20 + (level - 1) * 10;
}

void Game::updatelevel()
{
    level++;
}

void Game::spawnWolfByTimeAndLevel()
{
    int interval = 1000 - (level * 100);
    if (interval < 400) interval = 400;

    if (level > last_level) {
        point p;
        p.x = rand() % config.windWidth;
        p.y = (2 * config.toolBarHeight) + rand() % 200;
        if (wolf_count < 10) {
            wolves[wolf_count] = new Wolf(this, p, 30, 30, "images\\wolf.jpg");
            wolf_count++;
        }
        last_level = level;
        return;
    }

    if (timer % interval == 0) {
        if (wolf_count < (level + 2) && wolf_count < 10) {
            point p;
            p.x = rand() % config.windWidth;
            p.y = (2 * config.toolBarHeight) + rand() % 200;
            wolves[wolf_count] = new Wolf(this, p, 30, 30, "images\\wolf.jpg");
            wolf_count++;
        }
    }
}

clicktype Game::getMouseClick(int& x, int& y) const
{
    return pWind->GetMouseClick(x, y);
}

string Game::getSrting() const
{
    string Label;
    char Key;
    keytype ktype;
    pWind->FlushKeyQueue();
    while (true)
    {
        ktype = pWind->WaitKeyPress(Key);
        if (ktype == ESCAPE) return "";
        if (Key == 13)       return Label;
        if (Key == 8) {
            if (Label.size() > 0)
                Label.resize(Label.size() - 1);
        }
        else {
            Label += Key;
        }
        printMessage(Label);
    }
}

window* Game::CreateWind(int w, int h, int x, int y) const
{
    window* pW = new window(w, h, x, y);
    pW->SetBrush(config.bkGrndColor);
    pW->SetPen(config.bkGrndColor, 1);
    pW->DrawRectangle(0, 0, w, h);
    return pW;
}

window* Game::getWind() const { return pWind; }

void Game::createToolbar()
{
    point p = { 0, 0 };
    gameToolbar = new Toolbar(this, p, 0, config.toolBarHeight);
    gameToolbar->draw();
}

void Game::createBudgetbar()
{
    point p = { 0, config.toolBarHeight };
    gameBudgetbar = new Budgetbar(this, p, 0, config.toolBarHeight);
    gameBudgetbar->draw();
}

void Game::clearBudget() const
{
    pWind->SetPen(config.bkGrndColor, 1);
    pWind->SetBrush(config.bkGrndColor);
    pWind->DrawRectangle(config.windWidth - 500, config.toolBarHeight,
        config.windWidth, 2 * config.toolBarHeight);
}

void Game::printBudget(string msg) const
{
    clearBudget();
    int x = config.windWidth - 430;
    pWind->SetPen(config.penColor, 50);
    pWind->SetFont(24, BOLD, BY_NAME, "Arial");
    pWind->DrawString(x, config.toolBarHeight + 10, msg);
}

void Game::clearStatusBar() const
{
    pWind->SetPen(config.statusBarColor, 1);
    pWind->SetBrush(config.statusBarColor);
    pWind->DrawRectangle(0, config.windHeight - config.statusBarHeight,
        config.windWidth, config.windHeight);
    for (int i = 0; i < green_count; i++)
    {
        pWind->SetBrush(GREEN);
        pWind->SetPen(GREEN, 1);

        pWind->DrawRectangle(
            green_areas[i].x,
            green_areas[i].y,
            green_areas[i].x + 50,
            green_areas[i].y + 50
        );
    }
}

void Game::printMessage(string msg) const
{
    clearStatusBar();
    pWind->SetPen(config.penColor, 50);
    pWind->SetFont(24, BOLD, BY_NAME, "Arial");
    pWind->DrawString(10, config.windHeight - (int)(0.85 * config.statusBarHeight), msg);
}

void Game::go()
{
    int x, y;
    bool isExit = false;

    pWind->ChangeTitle("- - - - - - - - - - Farm Frenzy (CIE101-project) - - - - - - - - - -");

    do
    {
        if (!isPaused) {
            if (timer >= levelTime)
            {
                for (int i = 0; i < 50; i++) {
                    if (chicken_list[i]) { delete chicken_list[i]; chicken_list[i] = nullptr; }
                    if (cow_list[i]) { delete cow_list[i]; cow_list[i] = nullptr; }
                    if (food_list[i]) { delete food_list[i]; food_list[i] = nullptr; }
                }
                for (int i = 0; i < wolf_count; i++) {
                    if (wolves[i]) { delete wolves[i]; wolves[i] = nullptr; }
                }

                pWind->SetBrush(config.bkGrndColor);
                pWind->SetPen(config.bkGrndColor, 1);
                pWind->DrawRectangle(0, config.toolBarHeight * 2, config.windWidth, config.windHeight - config.statusBarHeight);

                pWind->SetPen(RED, 5);
                pWind->SetFont(50, BOLD, BY_NAME, "Arial");
                pWind->DrawString(config.windWidth / 2 - 150, config.windHeight / 2, "GAME OVER!");

                pWind->UpdateBuffer();
                Sleep(3000);
                return;
            }
            else
            {
                timer += 1;
            }
        }

        int minute = getminute();
        int second = getsecond();
        minute = remain_min();
        second = remain_sec();
        string time_string = "TIME: " + to_string(minute) + ":" + (second < 10 ? "0" : "") + to_string(second);
        string all_info = "READY...." + time_string;

        string level_string = "LEVEL: " + to_string(level);
        string goal = "GOAL: "
            + to_string(goal_chickens) + " chickens, "
            + to_string(goal_cows) + " cows, "
            + to_string(goal_water) + " water bottles";
        int total_chickens = ChickIcon::chick_count;
        int total_cows = cowIcon::cow_count;
        string animals_string = "ANIMALS: Chickens = "
            + to_string(total_chickens) + " , Cows = "
            + to_string(total_cows);
        string everything = all_info + "     " + level_string + "     " + goal + "     " + animals_string;

        if (!isPaused) {
            if (ChickIcon::chick_count >= goal_chickens &&
                cowIcon::cow_count >= goal_cows &&
                water_counter >= goal_water)
            {
                for (int i = 0; i < 50; i++) {
                    delete chicken_list[i]; chicken_list[i] = nullptr;
                    delete cow_list[i];     cow_list[i] = nullptr;
                    delete food_list[i];    food_list[i] = nullptr;
                }
                for (int i = 0; i < wolf_count; i++) {
                    delete wolves[i]; wolves[i] = nullptr;
                }
                wolf_count = 0;
                green_count = 0;
                for (int i = 0; i < 50; i++) {
                    egg_items[i].active = false;
                    milk_items[i].active = false;
                }
                ChickIcon::chick_count = 0;
                cowIcon::cow_count = 0;
                water_counter = 0;
                Chick::next_id = 0;
                Cow::next_id = 0;
                Wolf::speed += 0.25;
                updatelevel();
                updateGoals();
                levelTime = baseTime + ((level - 1) * scalingFactor);
                target_budget = budget + (level * 1000);
                printMessage("LEVEL " + to_string(level) + " START!");
            }
        }

        pWind->SetBrush(config.bkGrndColor);
        pWind->SetPen(config.bkGrndColor, 1);
        pWind->DrawRectangle(0, config.toolBarHeight * 2,
            config.windWidth, config.windHeight - config.statusBarHeight);

        for (int i = 0; i < 50; i++) {
            if (food_list[i] != nullptr)
                food_list[i]->draw();
        }

        if (!isPaused) {
            for (int i = 0; i < ChickIcon::chick_count; i++) {
                if (chicken_list[i] == nullptr) continue;
                for (int j = 0; j < 50; j++) {
                    if (food_list[j] == nullptr) continue;
                    int cx = chicken_list[i]->curr_pos.x;
                    int cy = chicken_list[i]->curr_pos.y;
                    int fx = food_list[j]->getX();
                    int fy = food_list[j]->getY();
                    if (cx < fx + 50 && cx + 40 > fx && cy < fy + 50 && cy + 40 > fy)
                        food_list[j]->collisionAction(chicken_list[i]);
                }
            }
            for (int i = 0; i < cowIcon::cow_count; i++) {
                if (cow_list[i] == nullptr) continue;
                for (int j = 0; j < 50; j++) {
                    if (food_list[j] == nullptr) continue;
                    int cx = cow_list[i]->curr_pos.x;
                    int cy = cow_list[i]->curr_pos.y;
                    int fx = food_list[j]->getX();
                    int fy = food_list[j]->getY();
                    if (cx < fx + 50 && cx + 40 > fx && cy < fy + 50 && cy + 40 > fy)
                        food_list[j]->collisionAction(cow_list[i]);
                }
            }
            for (int i = 0; i < 50; i++) {
                if (food_list[i] != nullptr && food_list[i]->isMarked()) {
                    delete food_list[i];
                    food_list[i] = nullptr;
                }
            }
        }

        for (int i = 0; i < ChickIcon::chick_count; i++) {
            if (chicken_list[i] != nullptr) {
                if (!isPaused) {
                    chicken_list[i]->updateProduction();
                    chicken_list[i]->moveStep();
                }
                chicken_list[i]->draw();

                if (chicken_list[i]->getEggCount() > 0) {
                    pWind->SetBrush(WHITE);
                    pWind->SetPen(DARKGRAY, 1);
                    pWind->DrawEllipse(chicken_list[i]->curr_pos.x + 52,
                        chicken_list[i]->curr_pos.y + 5,
                        chicken_list[i]->curr_pos.x + 72,
                        chicken_list[i]->curr_pos.y + 30);
                }
            }
        }

        for (int i = 0; i < cowIcon::cow_count; i++) {
            if (cow_list[i] != nullptr) {
                if (!isPaused) {
                    cow_list[i]->updateProduction();
                    cow_list[i]->moveStep();
                }
                cow_list[i]->draw();

                if (cow_list[i]->getMilkCount() > 0) {
                    pWind->SetBrush(LIGHTGRAY);
                    pWind->SetPen(DARKGRAY, 1);
                    pWind->DrawEllipse(cow_list[i]->curr_pos.x + 52,
                        cow_list[i]->curr_pos.y + 5,
                        cow_list[i]->curr_pos.x + 72,
                        cow_list[i]->curr_pos.y + 30);
                }
            }
        }

        if (!isPaused) {
            for (int w = 0; w < wolf_count; w++) {
                if (wolves[w] == nullptr || wolves[w]->isDead) continue;
                for (int c = 0; c < ChickIcon::chick_count; c++) {
                    if (chicken_list[c] == nullptr) continue;
                    int wx = wolves[w]->curr_pos.x;
                    int wy = wolves[w]->curr_pos.y;
                    int cx = chicken_list[c]->curr_pos.x;
                    int cy = chicken_list[c]->curr_pos.y;
                    if (wx < cx + 40 && wx + 30 > cx && wy < cy + 40 && wy + 30 > cy) {
                        delete chicken_list[c];
                        chicken_list[c] = nullptr;
                        ChickIcon::chick_count--;
                    }
                }
                for (int co = 0; co < cowIcon::cow_count; co++) {
                    if (cow_list[co] == nullptr) continue;
                    int wx = wolves[w]->curr_pos.x;
                    int wy = wolves[w]->curr_pos.y;
                    int cx = cow_list[co]->curr_pos.x;
                    int cy = cow_list[co]->curr_pos.y;
                    if (wx < cx + 40 && wx + 30 > cx && wy < cy + 40 && wy + 30 > cy) {
                        delete cow_list[co];
                        cow_list[co] = nullptr;
                        cowIcon::cow_count--;
                    }
                }
            }
        }

        if (!isPaused) {
            for (int i = 0; i < ChickIcon::chick_count; i++) {
                if (chicken_list[i] != nullptr && chicken_list[i]->getEggCount() > 0) {
                    int eggs = chicken_list[i]->collectEggs();
                    for (int e = 0; e < eggs; e++) {
                        for (int j = 0; j < 50; j++) {
                            if (!egg_items[j].active) {
                                egg_items[j].pos.x = chicken_list[i]->curr_pos.x + (rand() % 30);
                                egg_items[j].pos.y = chicken_list[i]->curr_pos.y + (rand() % 30);
                                egg_items[j].isEgg = true;
                                egg_items[j].active = true;
                                break;
                            }
                        }
                    }
                }
            }
            for (int i = 0; i < cowIcon::cow_count; i++) {
                if (cow_list[i] != nullptr && cow_list[i]->getMilkCount() > 0) {
                    int milk = cow_list[i]->collectMilk();
                    for (int m = 0; m < milk; m++) {
                        for (int j = 0; j < 50; j++) {
                            if (!milk_items[j].active) {
                                milk_items[j].pos.x = cow_list[i]->curr_pos.x + (rand() % 30);
                                milk_items[j].pos.y = cow_list[i]->curr_pos.y + (rand() % 30);
                                milk_items[j].isEgg = false;
                                milk_items[j].active = true;
                                break;
                            }
                        }
                    }
                }
            }
        }

        for (int i = 0; i < 50; i++) {
            if (egg_items[i].active) {
                pWind->SetBrush(WHITE);
                pWind->SetPen(BLACK, 1);
                pWind->DrawEllipse(egg_items[i].pos.x, egg_items[i].pos.y,
                    egg_items[i].pos.x + 20, egg_items[i].pos.y + 25);
                pWind->SetFont(10, BOLD, BY_NAME, "Arial");
                pWind->DrawString(egg_items[i].pos.x + 3, egg_items[i].pos.y + 5, "E");
            }
            if (milk_items[i].active) {
                pWind->SetBrush(LIGHTGRAY);
                pWind->SetPen(BLACK, 1);
                pWind->DrawRectangle(milk_items[i].pos.x, milk_items[i].pos.y,
                    milk_items[i].pos.x + 20, milk_items[i].pos.y + 25);
                pWind->SetFont(10, BOLD, BY_NAME, "Arial");
                pWind->DrawString(milk_items[i].pos.x + 3, milk_items[i].pos.y + 5, "M");
            }
        }

        warehouse->draw();

        for (int i = 0; i < wolf_count; i++)
        {
            if (wolves[i] != nullptr)
            {
                wolves[i]->draw();
                if (!isPaused) wolves[i]->move();
            }
        }

        printMessage(everything);

        string budget_string = "BUDGET = $" + to_string(budget);
        string water_string = "WATER BOTTLES = " + to_string(water_counter);
        if (isPaused) {
            printBudget("PAUSED!   " + water_string + "     " + budget_string);
        }
        else {
            printBudget(water_string + "     " + budget_string);
        }

        if (!isPaused) {
            pWind->UpdateBuffer();
        }
        Sleep(20);

        clicktype clk = getMouseClick(x, y);
        if (clk != NO_CLICK)
        {
            if (y >= 0 && y < config.toolBarHeight)
                isExit = gameToolbar->handleClick(x, y);
            else if (y >= config.toolBarHeight && y < 2 * config.toolBarHeight)
                isExit = gameBudgetbar->handleClick(x, y);
            else {
                bool collected = false;
                for (int i = 0; i < 50; i++) {
                    if (egg_items[i].active && x >= egg_items[i].pos.x && x <= egg_items[i].pos.x + 20 &&
                        y >= egg_items[i].pos.y && y <= egg_items[i].pos.y + 25) {
                        egg_items[i].active = false;
                        warehouse->addEgg();
                        printMessage("Egg collected!");
                        collected = true;
                        break;
                    }
                    if (milk_items[i].active && x >= milk_items[i].pos.x && x <= milk_items[i].pos.x + 20 &&
                        y >= milk_items[i].pos.y && y <= milk_items[i].pos.y + 25) {
                        milk_items[i].active = false;
                        warehouse->addMilk();
                        printMessage("Milk collected!");
                        collected = true;
                        break;
                    }
                }
                if (!collected) {
                    for (int i = 0; i < wolf_count; i++) {
                        if (wolves[i] != nullptr && !wolves[i]->isDead) {
                            if (wolves[i]->isClicked(x, y)) {
                                bool shouldKill = wolves[i]->hit();
                                int clicks = wolves[i]->getClickCount();
                                if (shouldKill) {
                                    wolves[i]->isDead = true;
                                    printMessage("Wolf killed! +$50 reward");
                                    budget += 50;
                                }
                                else {
                                    printMessage("Wolf hit! " + to_string(5 - clicks) + " more clicks to kill");
                                }
                                break;
                            }
                        }
                    }
                    if (warehouse->isClicked(x, y)) {
                        warehouse->openInventoryWindow();
                    }
                }
            }
        }
        for (int i = 0; i < wolf_count; i++) {
            if (wolves[i] != nullptr && wolves[i]->isDead) {
                delete wolves[i];
                wolves[i] = nullptr;
            }
        }

        if (!isPaused) {
            spawnWolfByTimeAndLevel();
        }

    } while (!isExit);
}