#include "Animal.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

int Chick::next_id = 0;
int Cow::next_id = 0;
int Wolf::next_id = 0;
float Wolf::speed = 2;

Animal::Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Drawable(r_pGame, r_point, r_width, r_height)
{
    image_path = img_path;
    curr_pos = r_point;
    curr_vel.x = 1;
    curr_vel.y = 1;
}

void Animal::draw() const
{
    window* pWind = pGame->getWind();
    pWind->DrawImage(image_path, curr_pos.x, curr_pos.y, width, height);
}

// ─── Chick ───────────────────────────────────────────────────────────────────

Chick::Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
    id = ++next_id;
    eatCounter = 0;
    eggCount = 0;
    productionTimer = 0;
}

void Chick::draw() const
{
    Animal::draw();
    window* pWind = pGame->getWind();
    pWind->SetPen(BLACK, 2);
    pWind->SetFont(14, BOLD, BY_NAME, "Arial");
    pWind->DrawString(curr_pos.x + width / 2 - 5, curr_pos.y - 15, to_string(id));
}

void Chick::moveStep()
{
    int min_x = 0;
    int max_x = config.windWidth;
    int min_y = 2 * config.toolBarHeight;
    int max_y = config.windHeight - config.statusBarHeight;

    if (!has_target) {
        target.x = rand() % (max_x - min_x + 1) + min_x;
        target.y = rand() % (max_y - min_y + 1) + min_y;
        has_target = true;
    }
    else {
        if (curr_pos.x < target.x) curr_pos.x += speed;
        else if (curr_pos.x > target.x) curr_pos.x -= speed;
        if (curr_pos.y < target.y) curr_pos.y += speed;
        else if (curr_pos.y > target.y) curr_pos.y -= speed;

        if (curr_pos.x <= min_x || curr_pos.x + width >= max_x ||
            curr_pos.y <= min_y || curr_pos.y + height >= max_y) {

            if (curr_pos.x <= min_x) target.x = max_x - width - 10;
            else if (curr_pos.x + width >= max_x) target.x = min_x + 10;
            if (curr_pos.y <= min_y) target.y = max_y - height - 10;
            else if (curr_pos.y + height >= max_y) target.y = min_y + 10;
        }

        float distance = sqrt(pow((float)(target.x - curr_pos.x), 2.0f)
            + pow((float)(target.y - curr_pos.y), 2.0f));
        if (distance < (float)speed)
            has_target = false;
    }
}

// ─── Cow ─────────────────────────────────────────────────────────────────────

Cow::Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
    id = ++next_id;
    eatCounter = 0;
    milkCount = 0;
    productionTimer = 0;
}

void Cow::draw() const
{
    Animal::draw();
    window* pWind = pGame->getWind();
    pWind->DrawString(curr_pos.x + width / 2 - 5, curr_pos.y - 15, to_string(id));
}

void Cow::moveStep()
{
    int min_x = 0;
    int max_x = config.windWidth;
    int min_y = 2 * config.toolBarHeight;
    int max_y = config.windHeight - config.statusBarHeight;

    if (!has_target) {
        target.x = rand() % (max_x - min_x + 1) + min_x;
        target.y = rand() % (max_y - min_y + 1) + min_y;
        has_target = true;
    }
    else {
        if (curr_pos.x < target.x) curr_pos.x += speed;
        else if (curr_pos.x > target.x) curr_pos.x -= speed;
        if (curr_pos.y < target.y) curr_pos.y += speed;
        else if (curr_pos.y > target.y) curr_pos.y -= speed;

        if (curr_pos.x <= min_x || curr_pos.x + width >= max_x ||
            curr_pos.y <= min_y || curr_pos.y + height >= max_y) {

            if (curr_pos.x <= min_x) target.x = max_x - width - 10;
            else if (curr_pos.x + width >= max_x) target.x = min_x + 10;
            if (curr_pos.y <= min_y) target.y = max_y - height - 10;
            else if (curr_pos.y + height >= max_y) target.y = min_y + 10;
        }

        float distance = sqrt(pow((float)(target.x - curr_pos.x), 2.0f)
            + pow((float)(target.y - curr_pos.y), 2.0f));
        if (distance < (float)speed)
            has_target = false;
    }
}

// ─── Wolf ────────────────────────────────────────────────────────────────────

Wolf::Wolf(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
    id = ++next_id;
    isDead = false;
}

bool Wolf::isClicked(int clickX, int clickY) const
{
    return (clickX >= curr_pos.x && clickX <= curr_pos.x + width &&
            clickY >= curr_pos.y && clickY <= curr_pos.y + height);
}

void Wolf::draw() const
{
    Animal::draw();
    window* pWind = pGame->getWind();
    pWind->DrawString(curr_pos.x + width / 2 - 5, curr_pos.y - 15, to_string(id));
}

void Wolf::moveStep()
{
    move();
}

void Wolf::move()
{
    int min_x = 0;
    int max_x = config.windWidth;
    int min_y = 2 * config.toolBarHeight;
    int max_y = config.windHeight - config.statusBarHeight;

    if (!has_target) {
        target.x = rand() % (max_x - min_x + 1) + min_x;
        target.y = rand() % (max_y - min_y + 1) + min_y;
        has_target = true;
    }
    else {
        if (curr_pos.x < target.x) curr_pos.x += speed;
        else if (curr_pos.x > target.x) curr_pos.x -= speed;
        if (curr_pos.y < target.y) curr_pos.y += speed;
        else if (curr_pos.y > target.y) curr_pos.y -= speed;

        if (curr_pos.x <= min_x || curr_pos.x + width >= max_x ||
            curr_pos.y <= min_y || curr_pos.y + height >= max_y) {

            if (curr_pos.x <= min_x) target.x = max_x - width - 10;
            else if (curr_pos.x + width >= max_x) target.x = min_x + 10;
            if (curr_pos.y <= min_y) target.y = max_y - height - 10;
            else if (curr_pos.y + height >= max_y) target.y = min_y + 10;
        }

        float distance = sqrt(pow((float)(target.x - curr_pos.x), 2.0f)
            + pow((float)(target.y - curr_pos.y), 2.0f));
        if (distance < (float)speed)
            has_target = false;
    }
}