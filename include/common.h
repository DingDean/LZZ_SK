#ifndef SKROBOT_SKROBOT_COMMON_H_
#define SKROBOT_SKROBOT_COMMON_H_

#include <algorithm>

namespace skrobot {

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

int GetCardValue(int card);
int GetCardColor(int card);
int GetCardLogicValue(int card);

bool SortByValueHelper(int a, int b);
void SortByValue(int *hand, int len);

int NumTrump (int *hand, int len);
int NumCardByValue (int *hand, int len, int value);

} //namespace skrobot

#endif /* ifndef SKROBOT_SKROBOT_COMMON_H_ */
