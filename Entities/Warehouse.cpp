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
    // إنشاء النافذة الجديدة
    window* pNewWind = new window(300, 250, 200, 200);

    const int eggBtnX = 200, eggBtnY = 55;
    const int milkBtnX = 200, milkBtnY = 95;

    // رسم زر الخروج (X) في أعلى اليمين عشان المستخدم يشوفه
    const int closeBtnX = 270, closeBtnY = 5;

    while (true) {
        // 1. مسح الشاشة ورسم المحتويات
        pNewWind->SetBrush(WHITE);
        pNewWind->SetPen(BLACK, 1);
        pNewWind->DrawRectangle(0, 0, 300, 250);

        pNewWind->SetPen(BLACK, 2);
        pNewWind->SetFont(20, BOLD, BY_NAME, "Arial");
        pNewWind->DrawString(80, 10, "INVENTORY");

        // زر الخروج المكتوب عليه X
        pNewWind->SetBrush(RED);
        pNewWind->DrawRectangle(closeBtnX, closeBtnY, closeBtnX + 25, closeBtnY + 20);
        pNewWind->SetPen(WHITE, 1);
        pNewWind->SetFont(12, BOLD, BY_NAME, "Arial");
        pNewWind->DrawString(closeBtnX + 7, closeBtnY + 2, "X");

        pNewWind->SetFont(14, BOLD, BY_NAME, "Arial");
        pNewWind->SetPen(BLACK, 1);

        // عرض البيض الحالي
        stringstream ss;
        ss << "Eggs: " << eggsCount;
        pNewWind->DrawString(30, 60, ss.str());

        // زر بيع البيض
        pNewWind->SetBrush(LIGHTGRAY);
        pNewWind->DrawRectangle(eggBtnX, eggBtnY, eggBtnX + 60, eggBtnY + 20);
        pNewWind->SetFont(10, BOLD, BY_NAME, "Arial");
        pNewWind->DrawString(eggBtnX + 5, eggBtnY + 5, "SELL ($5)");

        // عرض اللبن الحالي
        stringstream ss2;
        ss2 << "Milk: " << milkCount;
        pNewWind->DrawString(30, 100, ss2.str());

        // زر بيع اللبن
        pNewWind->SetBrush(LIGHTGRAY);
        pNewWind->DrawRectangle(milkBtnX, milkBtnY, milkBtnX + 60, milkBtnY + 20);
        pNewWind->SetFont(10, BOLD, BY_NAME, "Arial");
        pNewWind->DrawString(milkBtnX + 5, milkBtnY + 5, "SELL ($10)");

        pNewWind->UpdateBuffer();

        // 2. التعامل مع الضغطات بدون تجميد البرنامج
        int x, y;

        // ملحوظة: يفضل جداً استخدام WaitMouseClick لو المكتبة بتدعمها
        // لو مش بتدعمها، سيب GetMouseClick بس الكود اللي تحت هيمنع الكراش
        clicktype clk = pNewWind->GetMouseClick(x, y);

        if (clk == NO_CLICK) {
            Sleep(50); // بنريح الـ CPU لمدة 50 مللي ثانية عشان البرنامج ميهنجش
            continue;
        }

        // زر بيع البيض
        if (x >= eggBtnX && x <= eggBtnX + 60 && y >= eggBtnY && y <= eggBtnY + 20) {
            sellEggs(1); // استدعاء الدالة الأصلية عشان تحدث الـ budget والـ count بأمان
        }
        // زر بيع اللبن
        else if (x >= milkBtnX && x <= milkBtnX + 60 && y >= milkBtnY && y <= milkBtnY + 20) {
            sellMilk(1); // استدعاء الدالة الأصلية
        }
        // زر الخروج (X)
        else if (x >= closeBtnX && x <= closeBtnX + 25 && y >= closeBtnY && y <= closeBtnY + 20) {
            break;
        }
    }

    // حذف النافذة وتنظيف الذاكرة
    delete pNewWind;

    // إعادة رسم النافذة الرئيسية للعبة لتحديث الأرقام عليها فوراً
    pGame->getWind()->UpdateBuffer();
}