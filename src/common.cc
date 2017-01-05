#include "common.h"

namespace skrobot {

    int GetCardValue(int card) 
    {
        return card & kMaskValue;
    }

    int GetCardColor(int card) 
    {
        return card & kMaskColor;
    }

    int GetCardLogicValue (int card)
    {
        int color = GetCardColor(card);
        int value = GetCardValue(card);

        if (value >= 0x0E) 
            return value+2;
        return value <= 2 ? value+13 : value;
    }

    bool SortByValueHelper(int a, int b)
    {
        return GetCardValue(a) <= GetCardValue(b);
    }

    void SortByValue(int *hand, int len)
    {
        std::sort(hand, hand+len, SortByValueHelper);
    }

    bool IsTrump(int card)
    {
        if (card == 0x4E || card == 0x4F)
            return true;
        return false;
    }

    int NumTrump(int *hand, int len)
    {
        return std::count_if(hand, hand+len, [](int card) {return card == 0x4E || card == 0x4F;});
    }

    int NumCardByValue(int *hand, int len, int value)
    {
        return std::count_if(hand, hand+len, [value](int card) {return GetCardValue(card) == value;});
    }
} //namespace skrobot



