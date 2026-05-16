#pragma once
#include "../Core/Drawable.h"
struct point;
class Warehouse : public Drawable
{
private:
    int eggsCount;
    int milkCount;

public:
    Warehouse(Game* pGame, point ref);
    virtual void draw() const override;

    void addEgg() { eggsCount++; }
    void addMilk() { milkCount++; }
    int getEggs()  const { return eggsCount; }
    int getMilk()  const { return milkCount; }
    bool sellEggs(int count);
    bool sellMilk(int count);
    bool isClicked(int x, int y) const;
    void openInventoryWindow();
};