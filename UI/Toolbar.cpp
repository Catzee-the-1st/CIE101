#include "Toolbar.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
#include <fstream>
using namespace std;


ToolbarIcon::ToolbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Drawable(r_pGame, r_point, r_width, r_height)
{
    image_path = img_path;
}

void ToolbarIcon::draw() const
{
    window* pWind = pGame->getWind();
    pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}


RestartIcon::RestartIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void RestartIcon::onClick()
{
    pGame->restartGame();
}

//  PauseIcon 

PauseIcon::PauseIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void PauseIcon::onClick()
{
    pGame->setPaused(true);
    pGame->printMessage("Game Paused!");
}

//  ResumeIcon

ResumeIcon::ResumeIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void ResumeIcon::onClick()
{
    pGame->setPaused(false);
    pGame->printMessage("Game Resumed!");
}


SaveIcon::SaveIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void SaveIcon::onClick()
{
    pGame->printMessage("Saving...");

    ofstream out("save.txt");
    if (!out) {
        pGame->printMessage("Save FAILED – could not open save.txt");
        return;
    }

    // Budget & water
    out << pGame->getBudget() << "\n";
    out << pGame->water_counter << "\n";

    // Chickens
    out << ChickIcon::chick_count << "\n";
    for (int i = 0; i < ChickIcon::chick_count; i++) {
        if (pGame->chicken_list[i] != nullptr)
            out << pGame->chicken_list[i]->curr_pos.x << " "
            << pGame->chicken_list[i]->curr_pos.y << "\n";
    }

    // Cows
    out << cowIcon::cow_count << "\n";
    for (int i = 0; i < cowIcon::cow_count; i++) {
        if (pGame->cow_list[i] != nullptr)
            out << pGame->cow_list[i]->curr_pos.x << " "
            << pGame->cow_list[i]->curr_pos.y << "\n";
    }

    out.close();
    pGame->printMessage("Game saved.");
}

// LoadGameIcon 

LoadGameIcon::LoadGameIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void LoadGameIcon::onClick()
{
    pGame->printMessage("Loading...");

    ifstream in("save.txt");
    if (!in) {
        pGame->printMessage("Load FAILED – save.txt not found");
        return;
    }

    int b, wc;
    in >> b >> wc;
    pGame->setBudget(b);
    pGame->water_counter = wc;

    // Chickens
    int chickCount;
    in >> chickCount;
    ChickIcon::chick_count = chickCount;
    for (int i = 0; i < chickCount; i++) {
        int x, y;
        in >> x >> y;
        if (pGame->chicken_list[i] != nullptr) {
            pGame->chicken_list[i]->curr_pos.x = x;
            pGame->chicken_list[i]->curr_pos.y = y;
        }
    }

    // Cows
    int cowCount;
    in >> cowCount;
    cowIcon::cow_count = cowCount;
    for (int i = 0; i < cowCount; i++) {
        int x, y;
        in >> x >> y;
        if (pGame->cow_list[i] != nullptr) {
            pGame->cow_list[i]->curr_pos.x = x;
            pGame->cow_list[i]->curr_pos.y = y;
        }
    }

    in.close();
    pGame->printMessage("Game loaded.");
}

//  ExitIcon 

ExitIcon::ExitIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void ExitIcon::onClick()
{
    // Handled by handleClick() returning true — no extra work needed here
}

// Toolbar 

Toolbar::Toolbar(Game* r_pGame, point r_point, int r_width, int r_height)
    : Drawable(r_pGame, r_point, r_width, r_height)
{
    iconsImages[ICON_RESTART] = "images\\RESTART.jpg";
    iconsImages[ICON_PAUSE] = "images\\PAUSE.jpg";
    iconsImages[ICON_RESUME] = "images\\RESUME.jpg";
    iconsImages[ICON_SAVE] = "images\\SAVE.jpg";
    iconsImages[ICON_LOAD] = "images\\LOAD.jpg";
    iconsImages[ICON_EXIT] = "images\\toolbar_Exit.jpg";

    iconsList = new ToolbarIcon * [ICON_COUNT];

    point p = { 0, 0 };
    iconsList[ICON_RESTART] = new RestartIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_RESTART]); p.x += config.iconWidth;
    iconsList[ICON_PAUSE] = new PauseIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_PAUSE]);   p.x += config.iconWidth;
    iconsList[ICON_RESUME] = new ResumeIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_RESUME]);  p.x += config.iconWidth;
    iconsList[ICON_SAVE] = new SaveIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_SAVE]);    p.x += config.iconWidth;
    iconsList[ICON_LOAD] = new LoadGameIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_LOAD]);    p.x += config.iconWidth;
    iconsList[ICON_EXIT] = new ExitIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_EXIT]);
}

Toolbar::~Toolbar()
{
    for (int i = 0; i < ICON_COUNT; i++)
        delete iconsList[i];
    delete[] iconsList;
}

void Toolbar::draw() const
{
    for (int i = 0; i < ICON_COUNT; i++)
        iconsList[i]->draw();

    // Draw the bottom border of the toolbar
    window* pWind = pGame->getWind();
    pWind->SetPen(BLACK, 3);
    pWind->DrawLine(0, config.toolBarHeight, pWind->GetWidth(), config.toolBarHeight);
}

bool Toolbar::handleClick(int x, int y)
{
    if (x >= ICON_COUNT * config.iconWidth)
        return false;

    int clickedIconIndex = x / config.iconWidth;
    iconsList[clickedIconIndex]->onClick();

    return (clickedIconIndex == ICON_EXIT);
}