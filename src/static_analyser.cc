#include "static_analyser.h"

namespace skrobot {

bool InfoHandDistribution(int *hand, int len)
{
    
}

bool StaticAnalyserC::IsShunZi (int *hand, int len)
{
    if (len < 5 || len > kMaxHandLength)
        return false;

    SortByValue(hand, len);

    return true;
}

bool StaticAnalyserC::IsDouble(int *hand, int len) 
{
    if (len != 2)
        return false;

    SortByValue(hand, len);

    if (hand[1] == 0x4E || hand[1] == 0x4F) 
        return true;

    return GetCardValue(hand[0]) == GetCardValue(hand[1]);
}

bool StaticAnalyserC::IsTriple(int *hand, int len)
{
    if (len != 3)
        return false;

    SortByValue(hand, len);

    if (hand[len-1] != 0x4E && hand[len-1] != 0x4F)
        return GetCardValue(hand[0]) == GetCardValue(hand[1]) && GetCardValue(hand[1]) == GetCardValue(hand[2]);
    return this->IsDouble(hand, len -1);
}

bool StaticAnalyserC::IsDoubleLink(int *hand, int len)
{
    if (len < 6 || len % 2 != 0 || len > kMaxHandLength)
        return false;

    SortByValue(hand, len);

    int temp;

    for (int i = 0; i<len/2;i++)
    {
        int a = GetCardValue(hand[2*i]);
        int b = GetCardValue(hand[2*i + 1]);

        if (a != b)
            return false;

        if (i == 0) 
            temp = a;
        else
            if (a - temp != 1)
                return false;
            else
                temp = a;
    }

    return true;
}

bool StaticAnalyserC::IsTripleLink(int *hand, int len)
{
    if (len < 9 || len % 3 != 0 || len > kMaxHandLength)
        return false;

    SortByValue(hand, len);

    int temp;

    for (int i = 0; i<len/3;i++)
    {
        int a = GetCardValue(hand[3*i]);
        int b = GetCardValue(hand[3*i + 1]);
        int c = GetCardValue(hand[3*i + 2]);

        if (a != b || b != c)
            return false;

        if (i == 0) 
        {
            temp = a;
        }
        else
        {
            if (a - temp != 1)
                return false;
            else
                temp = a;
        }
    }

    return true;
}

bool StaticAnalyserC::IsBomb(int *hand, int len, bool is_sorted)
{
    if (len > kMaxHandLength)
        return false;

    if (len < 4)
        return this->IsBomb3W(hand, len);

    if (len == 4)
        if(this->IsBombTW(hand, len))
            return true;

    if (!is_sorted) 
        SortByValue(hand, len);

    if (hand[len -1] == 0x4E || hand[len - 1] == 0x4F) 
    {
        if ((len - 1) == 3)
            return this->IsTriple(hand, len-1);
        else
            return this->IsBomb(hand, len-1, true);
    }

    for (int i = 0; i < len - 1; i++)
    {
        if (GetCardValue(hand[i]) != GetCardValue(hand[i+1]))
            return this->IsBombLink(hand, len, i+1);
    }
    return true;
}

bool StaticAnalyserC::IsBomb3W(int *hand, int len)
{
    if (len != 3)
        return false;
    for (int i = 0; i < 3; i++)
    {
        if (GetCardColor(hand[i]) != 0x40)
            return false;
    }
    return true;
}

bool StaticAnalyserC::IsBombTW(int *hand, int len)
{
    if (len != 4)
        return false;
    for (int i = 0; i<4; i++)
    {
        if (GetCardColor(hand[i]) != 0x40)
            return false;
    }
    return true;
}

bool StaticAnalyserC::IsBombLink(int *hand, int len, int single_bomb_len)
{
    if (len % single_bomb_len != 0)
        return false;

    const int factor = len / single_bomb_len;

    SortByValue(hand, len);

    const int trump_num = NumTrump(hand, len);

    int previous_value;

    for (int i = 0; i<factor;i++)
    {
        int new_hand[single_bomb_len] = {};
        std::copy(hand + single_bomb_len*i, hand+single_bomb_len*(i+1), new_hand);
        if (!this->IsBomb(new_hand, single_bomb_len, true))
            return false;
        int current_value = GetCardValue(hand[single_bomb_len*i]);
        if (i == 0)
            previous_value = current_value;
        else
            if (current_value - previous_value != 1)
                return false;
            else 
                previous_value = current_value;
    }
    return true;
}

}
