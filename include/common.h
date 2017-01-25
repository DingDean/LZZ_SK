#ifndef SKROBOT_SKROBOT_COMMON_H_
#define SKROBOT_SKROBOT_COMMON_H_

#include <algorithm>
#include <vector>

namespace skrobot {

typedef std::vector<int> iVector;

// 司令分布描述符
struct TrumpDescriptor {
    int total_num;     // 司令总数
    int vice_trump_num; // 副司令数量
    int trump_num;      // 正司令数量
};


// 手牌类型
const int kHandTypeError        = 0;    //错误
const int kHandTypeSingle       = 1;    //单张
const int kHandTypeDouble       = 2;    //对子
const int kHandTypeTriple       = 3;    //三条
const int kHandTypeDoubleLink   = 4;    //对连
const int kHandTypeTripleLink   = 5;    //三连
const int kHandTypeShunzi       = 6;    //顺子
const int kHandTypeBomb         = 7;    //炸弹
const int kHandTypeBomb3W       = 8;    //三王炸
const int kHandTypeBombTW       = 9;    //天王炸
const int kHandTypeBombLink     = 10;   //排炸
//数值掩码
const int kMaskColor            = 0xF0; //花色掩码
const int kMaskValue            = 0x0F; //牌值掩码
//手牌信息
const int kMaxHandLength        = 27;   //玩家起始手牌数量
const int kDeckLength           = 108;  //游戏起始扑克数量

// 获得一张牌的实际点数
int GetCardValue(int card);
// 获得一张牌的花色
int GetCardColor(int card);
// 获得一张牌的逻辑点数
int GetCardLogicValue(int card);
// 将一副手牌按照每张牌的点数升序排列
void SortByValue (int *hand, int len);
// 将一副手牌按照每张牌的逻辑点数默认升序排列
void SortByLogicValue (int *hand, int len, bool isAscending);
// 获得手牌中司令的数量
int NumTrump (int *hand, int len);
// 获得手牌中司令的数量，带分布状况
void NumTrump (int *hand, int len, TrumpDescriptor *trump_desc);
// 获得手牌中点数为value的扑克牌的数量
int NumCardByValue (int *hand, int len, int value);

} //namespace skrobot

#endif /* ifndef SKROBOT_SKROBOT_COMMON_H_ */
