#pragma once
#include "../Core/Drawable.h"
#include "../Entities/Animal.h"
#include "../Config/GameConfig.h"
#include "Toolbar.h"
#include <random>

//  Random placement bounds for newly spawned animals
const int range_min_x = 50;
const int range_max_x = config.windWidth - 50;
const int range_min_y = (config.toolBarHeight * 2) + 50;
const int range_max_y = config.windHeight - config.statusBarHeight - 50;

//  Base icon

class BudgetbarIcon : public Drawable
{
public:
    string image_path;
    BudgetbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void draw() const override;
    virtual void onClick() = 0;
};

// Concrete purchase icons

class ChickIcon : public BudgetbarIcon
{
public:
    static int chick_count;
    
    ChickIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void onClick();
};

class cowIcon : public BudgetbarIcon
{
public:
    static int cow_count;
   
    cowIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void onClick();
};

class WaterIcon : public BudgetbarIcon
{
public:
    WaterIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void onClick();
};

// Icon order enum 

enum BUDGET_ICONS
{
    ICON_CHICK,
    Cow_icon,
    ICON_WATER,
    BUDGET_ITEM_COUNT   // Must be last
};

//  BudgetBar

class Budgetbar : public Drawable
{
private:
    BudgetbarIcon** iconsList;
    string iconsImages[BUDGET_ITEM_COUNT];

public:
    Budgetbar(Game* r_pGame, point r_point, int r_width, int r_height);
    ~Budgetbar();
    void draw() const override;
    bool handleClick(int x, int y);   // Returns true if exit should be triggered
};