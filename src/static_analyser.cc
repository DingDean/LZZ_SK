#include "static_analyser.h"

namespace skrobot {

bool StaticAnalyserC::IsShunZi (int *hand, int len)
{
    if (len < 5 || len > kMaxHandLength)
        return false;

    SortByValue(hand, len);

    int trump_num = NumTrump(hand, len);
    iVector distribution[2];
    this->DistributionByValue(hand, len-trump_num, distribution);

    int num_of_gap = NumberOfGap(distribution);
    int num_trump_needed = TrumpNeededForXLink(1, distribution);

    if (num_of_gap + num_trump_needed == trump_num)
    {
        FillInDescriptor(kHandTypeShunzi, GetCardValue(hand[0]), 1, len);
        return true;
    }
    return false;
}

bool StaticAnalyserC::IsDouble(int *hand, int len) 
{
    if (len != 2)
        return false;

    SortByValue(hand, len);

    if (hand[1] == 0x4E || hand[1] == 0x4F) 
    {
        FillInDescriptor(kHandTypeDouble,GetCardValue(hand[0]), 2, 1);
        return true;
    }

    if(GetCardValue(hand[0]) == GetCardValue(hand[1]))
    {
        FillInDescriptor(kHandTypeDouble,GetCardValue(hand[0]), 2, 1);
        return true;
    }
    return false;
}

bool StaticAnalyserC::IsTriple(int *hand, int len)
{
    if (len != 3)
        return false;

    SortByValue(hand, len);

    if (hand[len-1] != 0x4E && hand[len-1] != 0x4F)
    {
        if(GetCardValue(hand[0]) == GetCardValue(hand[1]) && GetCardValue(hand[1]) == GetCardValue(hand[2]))
        {
            FillInDescriptor(kHandTypeTriple, GetCardValue(hand[0]),3,1);
            return true;
        }
        return false;
    }
    if(this->IsDouble(hand, len -1))
    {
        FillInDescriptor(kHandTypeTriple, GetCardValue(hand[0]),3,1);
        return true;
    }
    return false;
}

bool StaticAnalyserC::IsDoubleLink(int *hand, int len)
{
    if (len < 6 || len % 2 != 0 || len > kMaxHandLength)
        return false;

    SortByValue(hand, len);

    int trump_num = NumTrump(hand, len);

    iVector distribution[2];
    this->DistributionByValue(hand, len-trump_num, distribution);

    int num_of_gap = NumberOfGap(distribution);
    int num_trump_needed = TrumpNeededForXLink(2, distribution);
    if (num_of_gap + num_trump_needed == trump_num)
    {
        FillInDescriptor(kHandTypeDoubleLink, GetCardValue(hand[0]), 2, len/2);
        return true;
    }
    return false;
}

bool StaticAnalyserC::IsTripleLink(int *hand, int len)
{
    if (len < 9 || len % 3 != 0 || len > kMaxHandLength)
        return false;

    SortByValue(hand, len);

    int trump_num = NumTrump(hand, len);

    iVector distribution[2];
    this->DistributionByValue(hand, len-trump_num, distribution);

    int num_of_gap = NumberOfGap(distribution);
    int num_trump_needed = TrumpNeededForXLink(3, distribution);

    if(num_of_gap + num_trump_needed == trump_num)
    {
        FillInDescriptor(kHandTypeTripleLink, GetCardValue(hand[0]), 3, len/3);
        return true;
    }
    return false;
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
        {
            if(this->IsTriple(hand, len-1))
            {
                FillInDescriptor(kHandTypeBomb, GetCardValue(hand[0]), len, 1);
                return true;
            }
            return false;
        }
        else
        {
            return this->IsBomb(hand, len-1, true);
        }
    }

    for (int i = 0; i < len - 1; i++)
    {
        if (GetCardValue(hand[i]) != GetCardValue(hand[i+1]))
            return this->IsBombLink(hand, len, i+1);
    }
    FillInDescriptor(kHandTypeBomb, GetCardValue(hand[0]), len, 1);
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
    FillInDescriptor(kHandTypeBomb3W, GetCardValue(hand[0]), 3, 1);
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
    FillInDescriptor(kHandTypeBombTW, GetCardValue(hand[0]), 4, 1);
    return true;
}

bool StaticAnalyserC::IsBombLink(int *hand, int len, int single_bomb_len)
{
    if (len % single_bomb_len != 0)
        return false;

    const int factor = len / single_bomb_len;

    SortByValue(hand, len);

    const int trump_num = NumTrump(hand, len);

    iVector distribution[2];
    this->DistributionByValue(hand, len-trump_num, distribution);

    int num_of_gap = NumberOfGap(distribution);
    int num_trump_needed = TrumpNeededForBomb(distribution);

    if(num_of_gap + num_trump_needed == trump_num)
    {
        FillInDescriptor(kHandTypeBombLink, GetCardValue(hand[0]), single_bomb_len, factor);
        return true;
    }
    return false;
}

bool StaticAnalyserC::GenHandDescriptor (int *hand, int len)
{
    if(len == 1)
    {
        FillInDescriptor(kHandTypeSingle, GetCardValue(hand[0]), 1, 1);
        return true;
    }
    SortByValue(hand, len);

    int possible_block_len = 1;
    for (int i=0; i<len-1; i++)
    {
        int cur  = GetCardValue(hand[i]);
        int next = GetCardValue(hand[i+1]);
        if (next != cur)
            break;
        possible_block_len++;
    }
    
    switch (possible_block_len) {
        case 1:
            return this->IsShunZi(hand, len);
        case 2:
            return (
                    this->IsDouble(hand, len) ||
                    this->IsDoubleLink(hand, len) ||
                    this->IsBomb(hand, len ,true)
                    );
        case 3:
            return (
                    this->IsTriple(hand, len) ||
                    this->IsTripleLink(hand, len)
                    );
        case 4:
            return this->IsBomb(hand, len, true);
        default:
            return false;
    }
}

bool StaticAnalyserC::React (int *hand, int len, int *out_hand)
{
    if (!this->GenHandDescriptor(hand, len))
        return false;
    this->GenOptions(hand, len);
}

bool StaticAnalyserC::GenOptions (int *hand, int len)
{
}

bool StaticAnalyserC::OptionsSingleCard (int *hand, int len, int input_card, iVector *output_options)
{
    int logic_value = GetCardLogicValue(input_card);
    for (int i = 0; i<len; i++)
    {
        if (GetCardLogicValue(hand[i]) > logic_value)
            output_options->push_back(hand[i]);
    }
    return true;
}

bool StaticAnalyserC::OptionsXples (int *hand, int len, int start_value, int comb_len, iVector *output_options)
{
    int logic_value = GetCardLogicValue(start_value);
    SortByLogicValue(hand, len, false);
    iVector trump_distr;

    NumTrump(hand, len, &trump_distr);
    int trump_total = trump_distr[0];
    int vice_trump_num = trump_distr[1];
    int trump_num = trump_distr[2];

    for (int i = 0; i<len;)
    {
        if (GetCardLogicValue(hand[i]) <= logic_value)
        {
            i++;
            continue;
        }

        int num = NumCardByValue(hand, len, GetCardValue(hand[i]));
        int minimum = comb_len - trump_total > 1 ? comb_len - trump_total : 1;

        if (hand[i] == 0x4E || hand[i] == 0x4F)
        {
            if (trump_total < comb_len)
            {
                i+=trump_total; 
                continue;
            }
            if (vice_trump_num >= comb_len)
            {
                for (int m=0; m<comb_len; m++)
                    output_options->push_back(0x4E);
            }
            if (trump_num >= comb_len)
            {
                for (int m=0; m<comb_len; m++)
                    output_options->push_back(0x4F);
            }
            i+=trump_total;
            continue;
        }

        if (num < minimum)
        {
            i += num; 
            continue;
        }

        for (int min = minimum; min<comb_len + 1; min++)
        {
            int trump_needed = comb_len - min;
            if (trump_needed == 0)
            {
                for (int z=0;z<min;z++)
                    output_options->push_back(hand[i+z]);
                continue;
            }
            if (vice_trump_num >= trump_needed)
            {
                for (int z=0;z<min;z++)
                    output_options->push_back(hand[i+z]);
                for (int m=0; m<trump_needed; m++)
                    output_options->push_back(0x4E);
            }
            if (trump_num >= trump_needed)
            {
                for (int z=0;z<min;z++)
                    output_options->push_back(hand[i+z]);
                for (int m=0; m<trump_needed; m++)
                    output_options->push_back(0x4F);
            }
            // TODO:正负司令混用
        }
        i+=num;
    }
    return true;
}

void StaticAnalyserC::OptionsBombs (int *hand, int len, int start_value, int block_len, int num_of_blocks, iVector *output_options)
{
    SortByLogicValue(hand, len, true);

    int min_logic_value = GetCardLogicValue(start_value);
    for (int i = 0; i<len;)
    {
        int num = NumCardByValue(hand, len, GetCardValue(hand[i]));
        int current_logic_value = GetCardLogicValue(hand[i]);
        if (current_logic_value <= min_logic_value)
        {
            i+=num;
            continue;
        }
        if (num >= block_len)
        {
            output_options->push_back(num);
            for (int j=0; j<num; j++)
                output_options->push_back(hand[i+j]);
        }
        i+=num;
    }
}

int StaticAnalyserC::NumberOfGap(iVector *distribution)
{
    iVector type_value = distribution[0];
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

int StaticAnalyserC::TrumpNeededForBomb(iVector *distribution)
{
    iVector type_num = distribution[1];

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

int StaticAnalyserC::TrumpNeededForXLink(int target, iVector *distribution)
{
    iVector type_num = distribution[1];
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

void StaticAnalyserC::DistributionByValue(int *hand, int len, iVector *distribution)
{
    SortByValue(hand, len);

    iVector value_num;
    iVector value_type;

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

void StaticAnalyserC::FillInDescriptor (int hand_type, int start_value, int block_len, int num_of_blocks)
{
    descriptor_.hand_type = hand_type;
    descriptor_.start_value = start_value;
    descriptor_.block_len = block_len;
    descriptor_.num_of_blocks = num_of_blocks;
}

}
