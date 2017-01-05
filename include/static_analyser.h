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
};

}// namespace skrobot



#endif/*STATIC_ANALYSER_H*/
