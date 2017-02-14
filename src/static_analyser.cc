#include "static_analyser.h"

namespace skrobot {

bool StaticAnalyserC::IsDouble(int *hand, int len) 
{
    if (len != 2)
        return false;

    SortByLogicValue(hand, len, true);

    if (hand[1] == 0x4E || hand[1] == 0x4F) 
    {
        FillInDescriptor(kHandTypeDouble,hand[0], 2, 1);
        return true;
    }

    if(GetCardValue(hand[0]) == GetCardValue(hand[1]))
    {
        FillInDescriptor(kHandTypeDouble,hand[0], 2, 1);
        return true;
    }
    return false;
}

bool StaticAnalyserC::IsTriple(int *hand, int len)
{
    if (len != 3)
        return false;

    SortByLogicValue(hand, len, true);

    if (hand[len-1] != 0x4E && hand[len-1] != 0x4F)
    {
        if(GetCardValue(hand[0]) == GetCardValue(hand[1]) && GetCardValue(hand[1]) == GetCardValue(hand[2]))
        {
            FillInDescriptor(kHandTypeTriple, hand[0],3,1);
            return true;
        }
        return false;
    }
    if(this->IsDouble(hand, len -1))
    {
        FillInDescriptor(kHandTypeTriple, hand[0],3,1);
        return true;
    }
    return false;
}

bool StaticAnalyserC::IsShunZi (int *hand, int len)
{
    if (len < 5 || len > kMaxHandLength)
        return false;

    SortByLogicValue(hand, len, true);

    // 2不能出现在对连中
    int twoNum = NumCardByLogic(hand, len, GetCardLogicValue(0x12));
    if (twoNum != 0)
        return false;

    int trump_num = NumTrump(hand, len);
    iVector distribution[2];
    this->DistributionByLogicValue(hand, len-trump_num, distribution);

    int num_of_gap = NumberOfGap(distribution);
    int num_trump_needed = TrumpNeededForXLink(1, distribution);

    if (num_of_gap + num_trump_needed == trump_num)
    {
        FillInDescriptor(kHandTypeShunzi, GetCardValue(hand[0]), 1, len);
        return true;
    }
    return false;
}

bool StaticAnalyserC::IsDoubleLink(int *hand, int len)
{
    if (len < 6 || len % 2 != 0 || len > kMaxHandLength)
        return false;

    SortByLogicValue(hand, len, true);
    
    // 2不能出现在对连中
    int twoNum = NumCardByLogic(hand, len, GetCardLogicValue(0x12));
    if (twoNum != 0)
        return false;

    int trump_num = NumTrump(hand, len);

    iVector distribution[2];
    this->DistributionByLogicValue(hand, len-trump_num, distribution);

    int num_of_gap = NumberOfGap(distribution);
    int num_trump_needed = TrumpNeededForXLink(2, distribution);
    if (num_of_gap*2 + num_trump_needed == trump_num)
    {
        FillInDescriptor(kHandTypeDoubleLink, hand[0], 2, len/2);
        return true;
    }
    return false;
}

bool StaticAnalyserC::IsTripleLink(int *hand, int len)
{
    if (len < 9 || len % 3 != 0 || len > kMaxHandLength)
        return false;

    SortByLogicValue(hand, len, true);

    // 2不能出现在三连中
    int twoNum = NumCardByLogic(hand, len, GetCardLogicValue(0x12));
    if (twoNum != 0)
        return false;

    int trump_num = NumTrump(hand, len);

    iVector distribution[2];
    this->DistributionByValue(hand, len-trump_num, distribution);

    int num_of_gap = NumberOfGap(distribution);
    int num_trump_needed = TrumpNeededForXLink(3, distribution);

    if(num_of_gap*3 + num_trump_needed == trump_num)
    {
        FillInDescriptor(kHandTypeTripleLink, hand[0], 3, len/3);
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
        SortByLogicValue(hand, len, true);

    if (hand[len -1] == 0x4E || hand[len - 1] == 0x4F) 
    {
        if ((len - 1) == 3)
        {
            if(this->IsTriple(hand, len-1))
            {
                FillInDescriptor(kHandTypeBomb, hand[0], len, 1);
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
    FillInDescriptor(kHandTypeBomb, hand[0], len, 1);
    return true;
}

bool StaticAnalyserC::IsBomb3W(int *hand, int len)
{
    if (len != 3)
        return false;
    for (int i = 0; i < 3; i++)
    {
        if (GetCardColor(hand[i]) != kTrumpColor)
            return false;
    }
    FillInDescriptor(kHandTypeBomb3W, hand[0], 3, 1);
    return true;
}

bool StaticAnalyserC::IsBombTW(int *hand, int len)
{
    if (len != 4)
        return false;
    for (int i = 0; i<4; i++)
    {
        if (GetCardColor(hand[i]) != kTrumpColor)
            return false;
    }
    FillInDescriptor(kHandTypeBombTW, hand[0], 4, 1);
    return true;
}

bool StaticAnalyserC::IsBombLink(int *hand, int len, int single_bomb_len)
{
    if (len % single_bomb_len != 0)
        return false;

    const int factor = len / single_bomb_len;

    if (factor < 3)
        return false;

    SortByLogicValue(hand, len, true);

    const int trump_num = NumTrump(hand, len);

    iVector distribution[2];
    this->DistributionByLogicValue(hand, len-trump_num, distribution);

    int num_of_gap = NumberOfGap(distribution);
    int num_trump_needed = TrumpNeededForBomb(distribution);

    if(num_of_gap*single_bomb_len + num_trump_needed == trump_num)
    {
        FillInDescriptor(kHandTypeBombLink, hand[0], single_bomb_len, factor);
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

void StaticAnalyserC::OptionsXples (int *hand, int len, int combo_card, int combo_len, iVector *output_options)
{
    if (len < 2)
        return;
    int combo_logic_value = GetCardLogicValue(combo_card);
    TrumpDescriptor trump_desc;
    NumTrump(hand, len, &trump_desc);

    for (int i = 0; i< len;)
    {
        int current_logic_value = GetCardLogicValue(hand[i]);
        int current_card_num    = NumCardByLogic(hand, len, GetCardLogicValue(hand[i]));
        if (current_logic_value <= combo_logic_value)
        {
            i+=current_card_num;
            continue;
        }

        if (current_logic_value >= GetCardLogicValue(0x4E))
        {
            if (trump_desc.total_num >= combo_len)
            {
                this->MakeXplesWithTrump(0, 0, combo_len, trump_desc, output_options);
            }
            return;
        }

        if (current_card_num + trump_desc.total_num < combo_len)
        {
            i+=current_card_num;
            continue;
        }

        for (int base_len=1; base_len<=current_card_num;base_len++)
        {
            if (base_len > combo_len)
                break;
            int trump_needed = combo_len -base_len ;
            if (trump_desc.total_num < trump_needed)
                continue;
            this->MakeXplesWithTrump(hand[i], base_len, trump_needed, trump_desc, output_options);
        }
        i+=current_card_num;
    }
}

void StaticAnalyserC::OptionsBombs (int *hand, int len, int start_value, int block_len, int num_of_blocks, iVector *output_options)
{
    SortByLogicValue(hand, len, true);
    TrumpDescriptor trump_desc;
    NumTrump(hand, len, &trump_desc);

    int min_logic_value = GetCardLogicValue(start_value);
    for (int i = 0; i<len;)
    {
        int current_logic_value = GetCardLogicValue(hand[i]);
        int num = NumCardByLogic(hand, len, current_logic_value);
        if (current_logic_value <= min_logic_value)
        {
            int possible_max_block_len = trump_desc.total_num + num;
            if (possible_max_block_len <= block_len)
                i+=num;continue;
            this->MakeBombsWithTrump(hand, len, i, num, block_len+1, possible_max_block_len, trump_desc, output_options);
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

void StaticAnalyserC::FindBomb3TW (int *hand, int len, iVector *output_options)
{
    TrumpDescriptor trump_desc;
    NumTrump(hand, len, &trump_desc);
    if (trump_desc.total_num >= 3)
    {
        int vice = trump_desc.vice_trump_num;
        int head = trump_desc.trump_num;

        if (vice == 2)
        {
            output_options->push_back(3);
            output_options->push_back(kTrumpVice);
            output_options->push_back(kTrumpVice);
            output_options->push_back(kTrumpHead);
        }

        if (head == 2)
        {
            output_options->push_back(3);
            output_options->push_back(kTrumpHead);
            output_options->push_back(kTrumpHead);
            output_options->push_back(kTrumpVice);
        }

        if (head == 2 && vice == 2)
        {
            output_options->push_back(4);
            for (int i=0;i<2;i++)
            {
                output_options->push_back(kTrumpHead);
                output_options->push_back(kTrumpVice);
            }
        }
    }
}


//TODO:2017-02-14
//完成逻辑
bool StaticAnalyserC::OptionsXLinks (int *hand, int len, int link_start_value, int link_block_len, int link_total_len, iVector * output_options)
{
    int target_start_logic_value = GetCardLogicValue(link_start_value);
    SortByLogicValue(hand, len, false);
    for (int i=0; i<len;)
    {
        int current_logic_value = GetCardLogicValue(hand[i]);
        int current_value_num   = NumCardByValue(hand, len, GetCardValue(hand[i]));
        if (current_logic_value <= target_start_logic_value)
        {
            i += current_value_num;
            continue;
        }

        // TODO:2017-02-10
        // 完成寻找连牌的逻辑
        bool is_good = true;
        int diff_card_needed = link_total_len/link_block_len;
        int previous_logic_value = current_logic_value;
        for (int j=current_value_num; j<diff_card_needed;)
        {
            int next_logic_value = GetCardLogicValue(hand[j]);
            int next_value_num   = NumCardByValue(hand, len, GetCardValue(hand[j]));
            if (next_logic_value - current_logic_value != 1)
            {
                is_good = false;
                break;
            }
            previous_logic_value = next_logic_value;
            j+=next_value_num;
        }
    }
}

//TODO:2017-02-14
//完成逻辑
bool StaticAnalyserC::React (int *hand, int len, int *out_hand)
{
    if (!this->GenHandDescriptor(hand, len))
        return false;
    this->GenOptions(hand, len);
}

//TODO:2017-02-14
//完成逻辑
bool StaticAnalyserC::GenOptions (int *hand, int len)
{
}

bool StaticAnalyserC::MakeXplesWithTrump (int base, int base_len, int trump_needed, TrumpDescriptor trump_desc, iVector *output_options)
{
    switch (trump_needed) {
        case 0:
            output_options->push_back(base_len);
            for (int j=0;j<base_len;j++)
                output_options->push_back(base);
            break;
        case 1:
            if (trump_desc.vice_trump_num > 0)
            {
                output_options->push_back(base_len);
                for (int j=0;j<base_len;j++)
                    output_options->push_back(base);
                output_options->push_back(0x4E);
            }
            if (trump_desc.trump_num > 0)
            {
                output_options->push_back(base_len);
                for (int j=0;j<base_len;j++)
                    output_options->push_back(base);
                output_options->push_back(0x4F);
            }
            break;
        case 2:
            if (trump_desc.vice_trump_num > 1)
            {
                output_options->push_back(base_len);
                for (int j=0;j<base_len;j++)
                    output_options->push_back(base);
                output_options->push_back(0x4E);
                output_options->push_back(0x4E);
            }
            if (trump_desc.trump_num > 1)
            {
                output_options->push_back(base_len);
                for (int j=0;j<base_len;j++)
                    output_options->push_back(base);
                output_options->push_back(0x4F);
                output_options->push_back(0x4F);
            }
            if (trump_desc.vice_trump_num > 0 && trump_desc.trump_num > 0)
            {
                output_options->push_back(base_len);
                for (int j=0;j<base_len;j++)
                    output_options->push_back(base);
                output_options->push_back(0x4F);
                output_options->push_back(0x4E);
            }
            break;
        case 3:
            if (trump_desc.vice_trump_num > 2)
            {
                output_options->push_back(base_len);
                for (int j=0;j<base_len;j++)
                    output_options->push_back(base);
                output_options->push_back(0x4E);
                output_options->push_back(0x4E);
                output_options->push_back(0x4E);
            }
            if (trump_desc.trump_num > 2)
            {
                output_options->push_back(base_len);
                for (int j=0;j<base_len;j++)
                    output_options->push_back(base);
                output_options->push_back(0x4F);
                output_options->push_back(0x4F);
                output_options->push_back(0x4F);
            }
            if (trump_desc.vice_trump_num > 0 && trump_desc.trump_num > 0 && trump_desc.total_num >= 3)
            {
                if (trump_desc.trump_num > 1)
                {
                    output_options->push_back(base_len);
                    for (int j=0;j<base_len;j++)
                        output_options->push_back(base);
                    output_options->push_back(0x4F);
                    output_options->push_back(0x4F);
                    output_options->push_back(0x4E);
                }
                if (trump_desc.vice_trump_num > 1)
                {
                    output_options->push_back(base_len);
                    for (int j=0;j<base_len;j++)
                        output_options->push_back(base);
                    output_options->push_back(0x4F);
                    output_options->push_back(0x4E);
                    output_options->push_back(0x4E);
                }
            }
            break;
        case 4:
            output_options->push_back(base_len);
            for (int j=0;j<base_len;j++)
                output_options->push_back(base);
            for (int j=0;j<2;j++)
            {
                output_options->push_back(0x4F);
                output_options->push_back(0x4E);
            }
            break;
        default:
            break;
    }
    return true;
}

bool StaticAnalyserC::FindCardsByValue(int *hand, int len, int card_value, int num)
{
    int occurence = 0;
    for (int i=0; i<len; i++)
    {
        int current_value = hand[i];
        if (current_value == card_value)
            occurence++;
        if (occurence == num)
            return true;
    }
    return false;
}

//TODO:2017-02-14
//完成逻辑
void StaticAnalyserC::MakeBombsWithTrump(
        int *hand,
        int len,
        int base_value_index, 
        int base_num, 
        int min_block_len, 
        int max_block_len, 
        TrumpDescriptor trump_desc, 
        iVector *output_options)
{
    for (int target_len=min_block_len; target_len<=max_block_len; target_len++)
    {
        int trump_needed = target_len - base_num;
        if (trump_desc.total_num < trump_needed)
            continue;
        for (int i=0;i<3;i++)
        {
            switch (i) {
                case 0:
                    if (trump_desc.vice_trump_num < trump_needed)
                        continue;
                    output_options->push_back(target_len);
                    for (int num=0;num<trump_needed;num++)
                        output_options->push_back(0x4E);
                    break;
                case 1:
                    if (trump_desc.trump_num < trump_needed)
                        continue;
                    output_options->push_back(target_len);
                    for (int num=0;num<trump_needed;num++)
                        output_options->push_back(0x4F);
                    break;
                case 2:
                    if (trump_needed < 2)
                        continue;
                    if (trump_desc.vice_trump_num > 0 && trump_desc.trump_num > 0)
                    {
                        output_options->push_back(target_len);
                        output_options->push_back(0x4E);
                        output_options->push_back(0x4F);
                        if (trump_needed -2 == 1)
                        {
                            if (trump_desc.vice_trump_num -1 > 0)
                                output_options->push_back(0x4E);
                            else
                                output_options->push_back(0x4F);
                        }
                        else if (trump_needed - 2 == 2)
                        {
                            output_options->push_back(0x4E);
                            output_options->push_back(0x4F);
                        }
                    }
                    break;
                default:
                    continue;
            }
            for (int z=0; z<base_num; z++)
                output_options->push_back(hand[base_value_index + z]);
        }
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

void StaticAnalyserC::DistributionByLogicValue (int *hand, int len, iVector *distribution)
{
    SortByLogicValue(hand, len, true);

    iVector value_num;
    iVector value_type;

    for (int i = 0; i<len;)
    {
        int current_value = GetCardLogicValue(hand[i]);
        int current_num = NumCardByLogic(hand, len, current_value);
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
    descriptor_.start_value = GetCardLogicValue(start_value);
    descriptor_.block_len = block_len;
    descriptor_.num_of_blocks = num_of_blocks;
}

}
