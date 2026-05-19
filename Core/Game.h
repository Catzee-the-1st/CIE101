#pragma once
#include "../CMUgraphicsLib/CMUgraphics.h"
#include "../UI/Toolbar.h"
#include "../UI/BudgetBar.h"
#include "../Entities/Animal.h"
#include "../Entities/Warehouse.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class GrassFood;

struct CollectableItem {
    point pos;
    bool isEgg;
    bool active;
};

class Game
{
private:
    window* pWind;
    Toolbar* gameToolbar;
    Budgetbar* gameBudgetbar;
    Warehouse* warehouse;
    CollectableItem egg_items[50];
    CollectableItem milk_items[50];
    int timer = 0;
    int game_frames = 50;
    int baseTime = (1 * 60) * game_frames;
    int scalingFactor = 15;
    bool isPaused;
    int game_over_timer = 0;
    int money;

public:
    void setPaused(bool p);
    bool getPaused() const;
    point green_areas[20];
    int green_count = 0;
    void spawnGreenArea();

    GrassFood* food_list[150];
    Chick* chicken_list[150];
    Cow* cow_list[150];

    void updateGoals();
    int goal_chickens = 10;
    int goal_cows = 5;
    int goal_water = 20;

    int budget = 2000 ;
    
    int water_counter = 0;
    int level = 1;
    int target_budget = budget + level * 100;
    bool level_completed = false;
    int levelTime = baseTime;

    Game();
    ~Game();

    int  getBudget()   const { return budget; }
    void setBudget(int b) { budget = b; }
    void increasewatercounter() { water_counter += 5; }
    int getminute() const { return timer / 3000; }
    int getsecond() const { return (timer / 50) % 60; }
    int game_time_sec() const { return (levelTime / 50) % 60; }
    int game_time_min() const { return levelTime / 3000; }
    int remain_sec() const { return ((levelTime - timer) / 50) % 60; }
    int remain_min() const { return (levelTime - timer) / 3000; }
    void updatelevel();
    void restartGame();
    void addFood(GrassFood* pFood);
    void removeGameObject(GrassFood* pFood);

    window* CreateWind(int w, int h, int x, int y) const;
    window* getWind() const;

    void createToolbar();
    void createBudgetbar();

    void clearBudget()            const;
    void printBudget(string msg)  const;
    void clearStatusBar()         const;
    void printMessage(string msg) const;

    clicktype getMouseClick(int& x, int& y) const;
    string    getSrting() const;

    void go();

    Wolf* wolves[10];
    int wolf_count = 0;
    void spawnWolfByTimeAndLevel();
    int last_level = 1;
    void addMoney(int amount );
    int getMoney()const;
};