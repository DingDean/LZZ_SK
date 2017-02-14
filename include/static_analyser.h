#ifndef STATIC_ANALYSER_H
#define STATIC_ANALYSER_H

#include "common.h"

namespace skrobot {

// 手牌描述符
struct HandDescriptor {
    int hand_type;
    int start_value; //一副手牌中最小的点数
    int block_len;   //一副手牌中相同点数的牌的数量
    int num_of_blocks; // 手牌中拥有的不同点数的数量
};

class StaticAnalyserC {
    public:
        // 判断一组手牌是否是顺子
        bool IsShunZi (int *hand, int len);
        // 判断一组手牌是否是对子
        bool IsDouble (int *hand, int len);
        // 判断一组手牌是否是三条
        bool IsTriple (int *hand, int len);
        // 判断一组手牌是否是对连
        bool IsDoubleLink (int *hand, int len);
        // 判断一组手牌是否是三连
        bool IsTripleLink (int *hand, int len);
        // 判断一组手牌是否是炸弹
        bool IsBomb (int *hand, int len, bool is_sorted);
        // 判断一组手牌是否是三王炸
        bool IsBomb3W (int *hand, int len);
        // 判断一组手牌是否是天王炸
        bool IsBombTW (int *hand, int len);
        // 判断一组手牌是否是排炸
        bool IsBombLink (int *hand, int len, int single_bomb_len);

        // 获得手牌描述符
        bool GenHandDescriptor (int *hand, int len);
        // TODO:逻辑＋测试
        // 获得一首牌
        bool React (int *hand, int len, int *out_hand);
        // TODO:逻辑＋测试
        // 获得可能的出牌选择
        bool GenOptions (int *hand, int len);
        // 获取一组手牌中各种点数的扑克牌的数量
        void DistributionByValue (int *hand, int len, iVector *distribution);
        // 获取一组手牌中各种逻辑点数的扑克牌的数量
        void DistributionByLogicValue (int *hand, int len, iVector *distribution);

        // 获得所有可以压制某张牌的散牌
        bool OptionsSingleCard (int *hand, int len, int input_card, iVector *output_options);
        void OptionsXples (int *hand, int len, int combo_card, int combo_len, iVector *output_options);
        // TODO:逻辑＋测试
        // 获得所有可以压制某连的手牌组合
        bool OptionsXLinks (int *hand, int len, int link_start_value, int link_block_len, int link_total_len, iVector *output_options);
        bool MakeXplesWithTrump (int base, int base_len, int trump_needed, TrumpDescriptor trump_desc, iVector *output_options);
        // TODO:逻辑+测试
        // 获得手牌中所有的炸弹
        void OptionsBombs (int *hand, int len, int min_value, int min_len, int strategy, iVector *output_options);
        // 从手牌中找到三王炸和天王炸
        void FindBomb3TW (int *hand, int len, iVector *output_options);
        // 从手牌中找到普通的炸弹
        void FindNormalBomb (int *hand, int len, iVector *output_options);
        // TODO:逻辑+测试
        // 用司令来凑炸弹
        void MakeBombsWithTrump (int *hand, int len, int base_value_index, int base_num, int min_block_len, int max_block_len, TrumpDescriptor trump_desc, iVector *output_options);

        // 判断一组手牌为了成为连牌，还差几个点数的牌
        int NumberOfGap (iVector *distribution);
        // 判断一副手牌需要多少司令来组成炸弹,只寻找普通炸弹而不是排炸
        int TrumpNeededForBomb (iVector *distribution);
        // 判断一副手牌需要多少司令来组成X连
        int TrumpNeededForXLink (int X, iVector *distribution);
        // 在手牌中寻找特定的几张牌
        bool FindCardsByValue(int *hand, int len, int card_value, int num);

        /*Getter and Setter*/
        inline HandDescriptor GetDescriptor () {return descriptor_;}
    private:
        HandDescriptor descriptor_;

        // 填充手牌描述符
        void FillInDescriptor (int hand_type, int start_value, int block_len, int num_of_blocks);
};

}// namespace skrobot



#endif/*STATIC_ANALYSER_H*/
