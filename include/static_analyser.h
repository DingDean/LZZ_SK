#ifndef STATIC_ANALYSER_H
#define STATIC_ANALYSER_H

#include "common.h"

namespace skrobot {

class StaticAnalyserC {
    public:
        // 将手牌按点数分割
        bool InfoHandDistribution(int *hand, int len);
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
    public:
        // 判断一组手牌的点数是否连续
        bool IsContinuous (std::vector<int> *distribution);
        // 判断一副手牌需要多少司令来组成炸弹
        int TrumpNeededForBomb (std::vector<int> *distribution);
        int TrumpNeededForXLink (std::vector<int> *distribution, int X);
    public:
        // 获取一组手牌中各种点数的扑克牌的数量
        void DistributionByValue(int *hand, int len, std::vector<int> *distribution);

};

}// namespace skrobot



#endif/*STATIC_ANALYSER_H*/
