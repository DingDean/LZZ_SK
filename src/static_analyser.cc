#include "static_analyser.h"

namespace skrobot {

bool StaticAnalyserC::IsShunZi (int *hand, int len)
{
    if (len < 5 || len > kMaxHandLength)
        return false;

    SortByValue(hand, len);

    int trump_num = NumTrump(hand, len);
    std::vector<int> distribution[2];
    this->DistributionByValue(hand, len-trump_num, distribution);

    int num_of_gap = this->NumberOfGap(distribution);
    int num_trump_needed = this->TrumpNeededForXLink(distribution, 1);

    return num_of_gap + num_trump_needed == trump_num;
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

    int trump_num = NumTrump(hand, len);

    std::vector<int> distribution[2];
    this->DistributionByValue(hand, len-trump_num, distribution);

    int num_of_gap = this->NumberOfGap(distribution);
    int num_trump_needed = this->TrumpNeededForXLink(distribution, 2);
    return num_of_gap + num_trump_needed == trump_num;
}

bool StaticAnalyserC::IsTripleLink(int *hand, int len)
{
    if (len < 9 || len % 3 != 0 || len > kMaxHandLength)
        return false;

    SortByValue(hand, len);

    int trump_num = NumTrump(hand, len);

    std::vector<int> distribution[2];
    this->DistributionByValue(hand, len-trump_num, distribution);

    int num_of_gap = this->NumberOfGap(distribution);
    int num_trump_needed = this->TrumpNeededForXLink(distribution, 3);

    return num_of_gap + num_trump_needed == trump_num;
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

    std::vector<int> distribution[2];
    this->DistributionByValue(hand, len-trump_num, distribution);

    int num_of_gap = this->NumberOfGap(distribution);
    int num_trump_needed = this->TrumpNeededForBomb(distribution);

    return num_of_gap + num_trump_needed == trump_num;
}

int StaticAnalyserC::NumberOfGap(std::vector<int> *distribution)
{
    std::vector<int> type_value = distribution[0];
    int max = type_value.size()-1;

    // 判断点数是否连续
    int gap = 0;
    for (int i=0; i<max; i++)
    {
        int diff = type_value[i+1] - type_value[i];
        if (diff == 1)
            continue;
        gap += diff-1;
    }
    return gap;
}

int StaticAnalyserC::TrumpNeededForBomb(std::vector<int> *distribution)
{
    std::vector<int> type_num = distribution[1];

    // 判断需要使用多少张司令
    int size = type_num.size();
    int max = type_num[size-1];
    if (max < 4) 
        return 9999;

    int num_trump_needed = 0;

    for (int i=0; i<size; i++)
    {
        int current_num = type_num[i];
        num_trump_needed += max-current_num;
    }
    return num_trump_needed;
}

int StaticAnalyserC::TrumpNeededForXLink(std::vector<int> *distribution, int target)
{
    std::vector<int> type_num = distribution[1];
    int size = type_num.size();
    int trump_needed = 0;
    for (int i=0; i<size; i++)
    {
        int num = type_num[i];
        if (num > target)
            return 9999;
        trump_needed += target-num;
    }
    return trump_needed;
}

void StaticAnalyserC::DistributionByValue(int *hand, int len, std::vector<int> *distribution)
{
    SortByValue(hand, len);

    std::vector<int> value_num;
    std::vector<int> value_type;

    for (int i = 0; i<len;)
    {
        int current_value = GetCardValue(hand[i]);
        int current_num = NumCardByValue(hand, len, current_value);
        value_num.push_back(current_num);
        value_type.push_back(current_value);
        i += current_num;
    }
    distribution[0] = value_type;
    distribution[1] = value_num;
}

}
